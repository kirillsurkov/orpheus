#include "orpheus/Engine.hpp"

#include <chrono>

namespace orpheus {
    Engine::Engine(const std::shared_ptr<interface::IWindow>&  window,
                   const std::shared_ptr<interface::IRender>&  render,
                   const std::shared_ptr<interface::IInput>&   input,
                   const std::shared_ptr<interface::IPhysics>& physics,
                   const std::shared_ptr<interface::IMath>&    math,
                   const std::shared_ptr<Scene>&               scene) :
        m_running(true),
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
        m_window->create("", 800, 600);

        m_scene->setWidth(800);
        m_scene->setHeight(600);

        m_renderExecutor.execute([&]() {
            m_renderContext = m_window->createContext();
            m_render->init();
        }).wait();
    }

    Engine::~Engine() {
        m_renderExecutor.execute([&]() { m_renderContext.reset(); }).wait();
        m_window->destroy();
    }

    void Engine::run() {
        auto last = std::chrono::system_clock::now();
        while (m_running) {
            auto now = std::chrono::system_clock::now();
            float delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() / 1000.0f;
            last = now;

            auto renderFuture  = m_renderExecutor.execute ([&]() {
                m_render->clear(0.0f, 0.0f, 0.0f, 1.0f);
                m_scene->draw(m_render);
                m_window->swapBuffers();
            });

            auto inputFuture   = m_renderExecutor.execute ([&]() { m_input->update();  });
            auto physicsFuture = m_physicsExecutor.execute([&]() { m_physics->update(delta); });

            physicsFuture.wait();
            inputFuture.wait();
            renderFuture.wait();

            if (m_input->isQuit()) {
                break;
            }

            m_physics->apply([&](interface::IPhysics::ID  id, const interface::IPhysics::State& state) { m_scene->setBody(id, state); });
            m_input->apply([&](interface::IInput::Key key, interface::IInput::State state) { m_scene->setKey(key, state); });

            m_scene->update(delta);
        }
    }
}
