#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"
#include "orpheus/Input/Keys.hpp"
#include "orpheus/Command/Engine/CommandKeyboard.hpp"
#include "orpheus/Command/Engine/CommandMouse.hpp"

#include <unordered_map>

namespace Orpheus::Input {
    class Manager : public Loggable {
    private:
        std::unordered_map<Key, bool> m_state;
        Dispatcher<Input::Key> m_keysDownDispatcher;
        Dispatcher<Input::Key> m_keysUpDispatcher;
        int m_mouseX;
        int m_mouseY;

        void onCommand(const Command::Engine::CommandKeyboard& command);
        void onCommand(const Command::Engine::CommandMouse& command);

    public:
        Manager();
        ~Manager();

        void unbindKeys();

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }

        template<class T>
        void bindKey(const Input::Key key, T&& function) {
            m_keysDownDispatcher.registerKey(key, [function]() { function(true); });
            m_keysUpDispatcher.registerKey(key,  [function]() { function(false); });
        }

        int getMouseX() const {
            return m_mouseX;
        }

        int getMouseY() const {
            return m_mouseY;
        }
    };
}
