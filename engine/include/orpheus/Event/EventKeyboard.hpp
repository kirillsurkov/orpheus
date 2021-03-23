#pragma once

#include "orpheus/Event/Event.hpp"
#include "orpheus/InputManager.hpp"

namespace Orpheus::Event {
    class EventKeyboard : public Event {
    private:
        Input::Key m_key;
        bool m_down;

    public:
        EventKeyboard(Input::Key key, bool down) :
            m_key(key),
            m_down(down)
        {
        }

        Input::Key getKey() const {
            return m_key;
        }

        bool isDown() const {
            return m_down;
        }

        virtual std::string getName() const override {
            return "EventKeyboard";
        }
    };
}
