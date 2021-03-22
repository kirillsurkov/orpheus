#pragma once

#include "orpheus/Event/Event.hpp"

namespace Orpheus::Event {
    class EventMouse : public Event {
    private:
        int m_x;
        int m_y;
        int m_dx;
        int m_dy;

    public:
        EventMouse(int x, int y, int dx, int dy) :
            m_x(x), m_y(y), m_dx(dx), m_dy(dy)
        {
        }

        virtual std::string getName() const override {
            return "EventMouse";
        }

        int getX() const {
            return m_x;
        }

        int getY() const {
            return m_y;
        }

        int getDX() const {
            return m_dx;
        }

        int getDY() const {
            return m_dy;
        }
    };
}
