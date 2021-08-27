#include "orpheus/Engine.hpp"

#include <chrono>
#include <iostream>

namespace orpheus {
    Engine::Engine(const std::shared_ptr<interface::IWindow>&  window,
                   const std::shared_ptr<interface::IRender>&  render,
                   const std::shared_ptr<interface::IInput>&   input,
                   const std::shared_ptr<interface::IPhysics>& physics,
                   const std::shared_ptr<interface::IMath>&    math,
                   const std::shared_ptr<Scene>&               scene) :
        m_running(true),
        m_fpsLimit(1000),
        m_window(window),
        m_render(render),
        m_renderExecutor("Render"),
        m_input(input),
        m_inputExecutor("Input"),
        m_physics(physics),
        m_physicsExecutor("Physics"),
        m_math(math),
        m_scene(scene)
    {
        m_scene->setWidth(1600);
        m_scene->setHeight(900);

        m_renderExecutor.start();
        m_inputExecutor.start();
        m_physicsExecutor.start();

        auto renderFut = m_renderExecutor.execute([&]() {
            m_window->create("TWG", 1600, 900);
            m_renderContext = m_window->createContext();
            m_render->init();
        });

        auto physicsFut = m_physicsExecutor.execute([&]() {
            if (m_physics) m_physics->init(1.0f / 10.0f);
        });

        physicsFut.wait();
        renderFut.wait();
    }

    Engine::~Engine() {
        auto renderFut = m_renderExecutor.execute([&]() {
            m_renderContext.reset();
            m_window->destroy();
        });

        auto physicsFut = m_physicsExecutor.execute([&]() {
            m_physics.reset();
        });

        physicsFut.wait();
        renderFut.wait();
    }

    void Engine::run() {
        auto last = std::chrono::system_clock::now();
        while (m_running) {
            auto now = std::chrono::system_clock::now();
            double delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() / 1000.0f;
            last = now;

            //std::cout << (1.0f / delta) << std::endl;

            auto renderFuture  = m_renderExecutor.execute([&]() {
                m_input->update();

                m_render->startFrame();
                m_scene->draw(m_render);
                m_render->endFrame();
                m_window->swapBuffers();
            });

            //auto inputFuture   = m_renderExecutor.execute ([&]() { m_input->update(); });
            auto physicsFuture = m_physicsExecutor.execute([&]() { if (m_physics) m_physics->update(delta); });

            physicsFuture.wait();
            //inputFuture.wait();
            renderFuture.wait();

            if (m_input->isQuit()) {
                break;
            }

            if (m_physics) m_physics->apply();

            m_scene->update(delta);

            std::this_thread::sleep_until(now + std::chrono::milliseconds(static_cast<std::uint32_t>(1000.0f / m_fpsLimit)));
        }
    }
}
