#include "orpheus/Engine.hpp"
#include "orpheus/Version.hpp"
#include "orpheus/Scene.hpp"

Orpheus::Engine::Engine(const Window::WindowPtr& window, const Render::RenderPtr& render) :
    m_window(window),
    m_render(render),
    m_alive(true)
{
    addScope("Engine");
    Log::info(this) << "Version " << Version::Major << "." << Version::Minor;

    m_render->init(m_window);

    m_window->registerCommand<Command::CommandQuit>(this);
    m_window->registerCommand<Command::CommandMouse>(m_inputManager);
    m_window->registerCommand<Command::CommandKeyboard>(m_inputManager);

    m_sceneBase = std::make_shared<Scene::Scene>(m_inputManager);
    m_sceneBase->registerCommand<Command::CommandScenePush>(this);
    m_sceneBase->registerCommand<Command::CommandScenePop>(this);
    m_sceneBase->registerCommand<Command::CommandTest>(this);
}

Orpheus::Engine::~Engine() {
}

bool Orpheus::Engine::isAlive() const {
    return m_alive;
}

void Orpheus::Engine::step(float delta) {
    m_window->pollEvents();

    if (m_sceneStack.size() > 0) {
        auto scene = m_sceneStack.top();
        for (const auto& entity : scene->getEntities()) {
            entity->update(delta);
        }
        scene->update(delta);
        m_render->drawScene(scene);
        m_window->swapBuffers();
    } else {
        postCommand<Command::CommandQuit>();
    }
}

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandQuit>&/* command*/) {
    m_alive = false;
}

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandScenePush>& command) {
    auto index = command->getTypeIndex();
    auto it = m_sceneCache.find(index);
    if (it == m_sceneCache.end()) {
        it = m_sceneCache.emplace(index, command->createScene(m_sceneBase)).first;
    }
    m_sceneStack.push(it->second);
    m_inputManager.unbindKeys();
    m_sceneStack.top()->onShow();
}

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandScenePop>&/* command*/) {
    m_sceneStack.pop();
    if (m_sceneStack.size() > 0) {
        m_inputManager.unbindKeys();
        m_sceneStack.top()->onShow();
    }
}

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandTest>& command) {
    Log::info(this) << command->getMessage();
}
