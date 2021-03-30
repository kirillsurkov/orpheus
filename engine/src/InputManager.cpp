#include "orpheus/InputManager.hpp"
#include "orpheus/Command/CommandKeyboard.hpp"
#include "orpheus/Command/CommandMouse.hpp"
#include "orpheus/Exception.hpp"

Orpheus::Input::Manager::Manager() {
    addScope("InputManager");
}

Orpheus::Input::Manager::~Manager() {
}

void Orpheus::Input::Manager::onCommand(const std::shared_ptr<Command::CommandKeyboard>& command) {
    bool down = command->isDown();
    auto key = command->getKey();

    auto it = m_state.find(key);
    if (it == m_state.end() || it->second != down) {
        m_state[key] = down;

        if (command->isDown()) {
            m_keysDownDispatcher.dispatch(command->getKey());
        } else {
            m_keysUpDispatcher.dispatch(command->getKey());
        }
    }
}

void Orpheus::Input::Manager::onCommand(const std::shared_ptr<Command::CommandMouse>& command) {
    Log::info(this) << command->getX() << ", " << command->getY() << ", " << command->getDX() << ", " << command->getDY();
}

void Orpheus::Input::Manager::unbindKeys() {
    m_keysDownDispatcher.clear();
    m_keysUpDispatcher.clear();
}
