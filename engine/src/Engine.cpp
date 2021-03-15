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

void Orpheus::Engine::quit() {
    m_alive = false;
}

void Orpheus::Engine::loop() {
    while (m_alive) {
        m_window->pollEvents();
        //m_render->clearColor(1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f);
        m_render->clear();
        m_window->swapBuffers();
    }
}

void Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventQuit>&/* event*/) {
    quit();
}

void Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventKeyboard>& event) {
    if (event->getKey() == Event::EventKeyboard::Key::ESC) {
        quit();
    }
}

void Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventMouse>& event) {

}
