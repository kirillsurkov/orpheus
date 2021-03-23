#include "orpheus/InputManager.hpp"
#include "orpheus/Event/EventKeyboard.hpp"
#include "orpheus/Event/EventMouse.hpp"
#include "orpheus/Exception.hpp"

Orpheus::Input::Manager::Manager() {
    addScope("InputManager");
}

Orpheus::Input::Manager::~Manager() {
}

void Orpheus::Input::Manager::onEvent(const std::shared_ptr<Event::EventKeyboard>& event) {
    bool down = event->isDown();
    auto key = event->getKey();

    auto it = m_state.find(key);
    if (it == m_state.end() || it->second != down) {
        m_state[key] = down;
        if (!m_eventsDispatcher.dispatch(event)) {
            throw Exception(this, "Event '" + event->getName() + "' is not supported within Input::Manager");
        }
    }
}
