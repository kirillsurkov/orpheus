#include "orpheus/Engine.hpp"

Orpheus::Engine::Engine(const Window::WindowPtr& window, const Render::RenderPtr& render) :
    m_window(window),
    m_render(render),
    m_alive(true)
{
    addScope("Engine");

    m_render->init(m_window);
    m_render->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_window->registerEventType<Event::EventQuit>(this);
    m_window->registerEventType<Event::EventKeyboard>(this);
    m_window->registerEventType<Event::EventMouse>(this);
}

Orpheus::Engine::~Engine() {
}

bool Orpheus::Engine::isAlive() const {
    return m_alive;
}

void Orpheus::Engine::loadScene(const std::shared_ptr<Scene::Scene>& scene) {
    Log::info(scene) << "loaded";
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
