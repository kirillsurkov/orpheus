#include "orpheus/Engine.hpp"
#include "orpheus/Version.hpp"
#include "orpheus/Scene.hpp"
#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Entity/EntityCommand.hpp"

Orpheus::Engine::Engine(Caches& caches, Window::Window& window, Render::Render& render) :
    m_caches(caches),
    m_window(window),
    m_render(render),
    m_alive(true)
{
    addScope("Engine");
    Log::info(this) << "Version " << Version::Major << "." << Version::Minor;

    m_window.registerCommand<Command::Engine::CommandQuit>(this);
    m_window.registerCommand<Command::Engine::CommandMouse>(m_inputManager);
    m_window.registerCommand<Command::Engine::CommandKeyboard>(m_inputManager);

    auto& vertexBufferCache = m_caches.vertexBufferCache();

    m_sceneBase = std::make_shared<Scene::Scene>(window.getWidth(), window.getHeight(), m_inputManager, vertexBufferCache);
    m_sceneBase->registerCommand<Command::Game::CommandScenePush>(this);
    m_sceneBase->registerCommand<Command::Game::CommandScenePop>(this);
    m_sceneBase->registerCommand<Command::Game::CommandTest>(this);

    auto& rectLB = vertexBufferCache.add("rect_left_bottom", 2);
    rectLB.addPoint(0.0f, 0.0f);
    rectLB.addPoint(1.0f, 0.0f);
    rectLB.addPoint(1.0f, 1.0f);
    rectLB.addPoint(1.0f, 1.0f);
    rectLB.addPoint(0.0f, 0.0f);
    rectLB.addPoint(0.0f, 1.0f);

    auto& rectCC = vertexBufferCache.add("rect_center_center", 2);
    rectCC.addPoint(-0.5f, -0.5f);
    rectCC.addPoint( 0.5f, -0.5f);
    rectCC.addPoint( 0.5f,  0.5f);
    rectCC.addPoint( 0.5f,  0.5f);
    rectCC.addPoint(-0.5f, -0.5f);
    rectCC.addPoint(-0.5f,  0.5f);

    auto& cubeCC = vertexBufferCache.add("cube_center_center", 3);
    for (std::size_t i = 0; i < 6; i++) {
        float data[3];
        std::size_t anchor = i / 2;
        data[anchor] = 0.5f * (i % 2 ? -1 : 1);
        for (std::size_t j = 0; j < 6; j++) {
            data[(anchor + 1) % 3] = 0.5f * ((j + 4) % 6 < 3 ? -1 : 1);
            data[(anchor + 2) % 3] = 0.5f * ((j + 5) % 6 < 3 ? -1 : 1);
            cubeCC.addPoint(data[0], data[1], data[2]);
        }
    }
}

Orpheus::Engine::~Engine() {
}

bool Orpheus::Engine::isAlive() const {
    return m_alive;
}

void Orpheus::Engine::step(float delta) {
    m_window.pollEvents();

    if (m_sceneStack.size() > 0) {
        auto scene = m_sceneStack.top();
        scene->step(delta);
        scene->draw(m_render);
        m_window.swapBuffers();
    } else {
        postCommand<Command::Engine::CommandQuit>();
    }
}

void Orpheus::Engine::onCommand(const Command::Engine::CommandQuit&/* command*/) {
    m_alive = false;
}

void Orpheus::Engine::onCommand(const Command::Game::CommandScenePush& command) {
    auto index = command.getTypeIndex();
    auto it = m_sceneCache.find(index);
    if (it == m_sceneCache.end()) {
        it = m_sceneCache.emplace(index, command.createScene(*m_sceneBase)).first;
    }
    m_sceneStack.push(it->second);
    m_inputManager.unbindKeys();
    m_sceneStack.top()->show();
}

void Orpheus::Engine::onCommand(const Command::Game::CommandScenePop&/* command*/) {
    m_sceneStack.pop();
    if (m_sceneStack.size() > 0) {
        m_inputManager.unbindKeys();
        m_sceneStack.top()->show();
    }
}

void Orpheus::Engine::onCommand(const Command::Game::CommandTest& command) {
    Log::info(this) << command.getMessage();
}
