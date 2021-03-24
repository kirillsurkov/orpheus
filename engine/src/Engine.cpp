#include "orpheus/Engine.hpp"
#include "orpheus/Version.hpp"

Orpheus::Engine::Engine(const Window::WindowPtr& window, const Render::RenderPtr& render) :
    m_window(window),
    m_render(render),
    m_alive(true)
{
    addScope("Engine");
    Log::info(this) << "Version " << Version::Major << "." << Version::Minor;

    m_render->init(m_window);
    m_render->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_window->registerEventType<Event::EventQuit>(this);
    m_window->registerEventType<Event::EventMouse>(this);

    m_window->registerEventType<Event::EventKeyboard>(m_inputManager);
    m_inputManager.registerEventType<Event::EventKeyboard>(this);

    m_sceneBase = std::make_shared<Scene::Scene>();
    m_sceneBase->registerEventType<Event::EventLoadScene>(this);
    m_sceneBase->registerEventType<Event::EventTest>(this);
}

Orpheus::Engine::~Engine() {
}

bool Orpheus::Engine::isAlive() const {
    return m_alive;
}

void Orpheus::Engine::loadScene(const std::shared_ptr<Scene::Scene>& scene) {
    m_sceneStack.push(scene);
}

void Orpheus::Engine::step() {
    for (const auto& event : m_events) {
        event();
    }
    m_events.clear();
    m_window->pollEvents();

    m_render->clear();
    m_window->swapBuffers();
}

void Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventQuit>&/* event*/) {
    m_alive = false;
}

void Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventKeyboard>& event) {
    if (event->isDown()) {
        m_keysDownDispatcher.dispatch(event->getKey());
    } else {
        m_keysUpDispatcher.dispatch(event->getKey());
    }
}

void Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventMouse>& event) {
    Log::info(this) << event->getX() << ", " << event->getY() << ", " << event->getDX() << ", " << event->getDY();
}

void Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventLoadScene>& event) {
    auto index = event->getTypeIndex();
    auto it = m_sceneCache.find(index);
    if (it == m_sceneCache.end()) {
        it = m_sceneCache.emplace(index, event->createScene(m_sceneBase)).first;
    }
    loadScene(it->second);
}

void Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventTest>& event) {
    Log::info(this) << event->getMessage();
}
