#pragma once

#include "orpheus/Event/Event.hpp"

namespace Orpheus::Event {
    class EventKeyboard : public Event {
    public:
        enum class Key {
            ESC,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
            N1, N2, N3, N4, N5, N6, N7, N8, N9, N0,
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

    private:
        Key m_key;
        bool m_down;

    public:
        EventKeyboard(Key key, bool down) :
            m_key(key),
            m_down(down)
        {
        }

        Key getKey() const {
            return m_key;
        }

        bool isDown() const {
            return m_down;
        }
    };
}
