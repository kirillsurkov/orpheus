#include "orpheus/Engine.hpp"
#include "orpheus/Version.hpp"
#include "orpheus/Event/EventQuit.hpp"
#include "orpheus/Event/EventKeyboard.hpp"
#include "orpheus/Event/EventMouse.hpp"
#include "orpheus/Event/EventScenePush.hpp"
#include "orpheus/Event/EventScenePop.hpp"
#include "orpheus/Event/EventTest.hpp"

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

    m_sceneBase = std::make_shared<Scene::Scene>(*this);
    m_sceneBase->registerEventType<Event::EventScenePush>(this);
    m_sceneBase->registerEventType<Event::EventScenePop>(this);
    m_sceneBase->registerEventType<Event::EventTest>(this);
}

Orpheus::Engine::~Engine() {
}

template<>
bool Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventQuit>&/* event*/) {
    m_alive = false;
    return true;
}

template<>
bool Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventKeyboard>& event) {
    if (event->isDown()) {
        m_keysDownDispatcher.dispatch(event->getKey());
    } else {
        m_keysUpDispatcher.dispatch(event->getKey());
    }
    return true;
}

template<>
bool Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventMouse>& event) {
    Log::info(this) << event->getX() << ", " << event->getY() << ", " << event->getDX() << ", " << event->getDY();
    return true;
}

template<>
bool Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventScenePush>& event) {
    auto index = event->getTypeIndex();
    auto it = m_sceneCache.find(index);
    if (it == m_sceneCache.end()) {
        it = m_sceneCache.emplace(index, event->createScene(m_sceneBase)).first;
    }
    pushScene(it->second);
    return true;
}

template<>
bool Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventScenePop>&/* event*/) {
    return popScene();
}

template<>
bool Orpheus::Engine::onEvent(const std::shared_ptr<Event::EventTest>& event) {
    Log::info(this) << event->getMessage();
    return true;
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

bool Orpheus::Engine::popScene() {
    m_sceneStack.pop();

    if (m_sceneStack.size() > 0) {
        m_keysDownDispatcher.clear();
        m_keysUpDispatcher.clear();
        m_sceneStack.top()->onShow();
        return true;
    }

    onEvent(std::make_shared<Event::EventQuit>());
    return false;
}

void Orpheus::Engine::step() {
    for (const auto& event : m_events) {
        if (!event()) {
            return;
        }
    }
    m_events.clear();
    m_window->pollEvents();

    m_render->clear();
    m_window->swapBuffers();
}
