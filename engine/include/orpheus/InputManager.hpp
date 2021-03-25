#pragma once

#include "Log.hpp"
#include "orpheus/Dispatcher.hpp"

#include <unordered_map>

namespace Orpheus::Command {
    class CommandKeyboard;
}

namespace Orpheus::Input {
    enum class Key {
        ESC,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        N1, N2, N3, N4, N5, N6, N7, N8, N9, N0,
        NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9, NP0,
        Q, W, E, R, T, Y, U, I, O, P,
        A, S, D, F, G, H, J, K, L,
        Z, X, C, V, B, N, M,
        SPACE,
        ENTER,
        UP, DOWN, LEFT, RIGHT,
        L_CTRL, L_SHIFT, L_ALT,
        R_CTRL, R_SHIFT, R_ALT,
        UNKNOWN
    };

    class Manager : public Loggable {
    private:
        CommandDispatcher m_commandDispatcher;
        std::unordered_map<Key, bool> m_state;

        void onCommand(const std::shared_ptr<Command::CommandKeyboard>& command);

    public:
        Manager();
        ~Manager();

        template<class T, class U>
        void registerCommand(U&& receiver) {
            m_commandDispatcher.registerCommand<T>(std::forward<U>(receiver));
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
