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
    m_window->registerCommand<Command::CommandMouse>(this);
    m_window->registerCommand<Command::CommandKeyboard>(m_inputManager);

    m_inputManager.registerCommand<Command::CommandKeyboard>(this);

    m_sceneBase = std::make_shared<Scene::Scene>(*this);
    m_sceneBase->registerCommand<Command::CommandScenePush>(this);
    m_sceneBase->registerCommand<Command::CommandScenePop>(this);
    m_sceneBase->registerCommand<Command::CommandTest>(this);
}

Orpheus::Engine::~Engine() {
}

bool Orpheus::Engine::isAlive() const {
    return m_alive;
}

void Orpheus::Engine::pushScene(const std::shared_ptr<Scene::Scene>& scene) {
    m_sceneStack.push(scene);

    m_keysDownDispatcher.clear();
    m_keysUpDispatcher.clear();
    m_sceneStack.top()->onShow();
}

void Orpheus::Engine::popScene() {
    m_sceneStack.pop();

    if (m_sceneStack.size() > 0) {
        m_keysDownDispatcher.clear();
        m_keysUpDispatcher.clear();
        m_sceneStack.top()->onShow();
    }
}

void Orpheus::Engine::step(float delta) {
    m_window->pollEvents();

    if (m_sceneStack.size() > 0) {
        auto scene = m_sceneStack.top();
        scene->updateEntities(delta);
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

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandKeyboard>& command) {
    if (command->isDown()) {
        m_keysDownDispatcher.dispatch(command->getKey());
    } else {
        m_keysUpDispatcher.dispatch(command->getKey());
    }
}

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandMouse>& command) {
    Log::info(this) << command->getX() << ", " << command->getY() << ", " << command->getDX() << ", " << command->getDY();
}

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandScenePush>& command) {
    auto index = command->getTypeIndex();
    auto it = m_sceneCache.find(index);
    if (it == m_sceneCache.end()) {
        it = m_sceneCache.emplace(index, command->createScene(m_sceneBase)).first;
    }
    pushScene(it->second);
}

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandScenePop>&/* command*/) {
    popScene();
}

void Orpheus::Engine::onCommand(const std::shared_ptr<Command::CommandTest>& command) {
    Log::info(this) << command->getMessage();
}
