#include "orpheus/Input/InputManager.hpp"
#include "orpheus/Exception.hpp"

Orpheus::Input::Manager::Manager() {
    addScope("InputManager");
}

Orpheus::Input::Manager::~Manager() {
}

void Orpheus::Input::Manager::onCommand(const Command::Engine::CommandKeyboard& command) {
    bool down = command.isDown();
    auto key = command.getKey();

    auto it = m_state.find(key);
    if (it == m_state.end() || it->second != down) {
        m_state[key] = down;

        if (command.isDown()) {
            m_keysDownDispatcher.dispatch(command.getKey());
        } else {
            m_keysUpDispatcher.dispatch(command.getKey());
        }
    }
}

void Orpheus::Input::Manager::onCommand(const Command::Engine::CommandMouse& command) {
    m_mouseX = command.getX();
    m_mouseY = command.getY();
}

void Orpheus::Input::Manager::unbindKeys() {
    m_keysDownDispatcher.clear();
    m_keysUpDispatcher.clear();
}
