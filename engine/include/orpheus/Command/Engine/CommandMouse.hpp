#pragma once

#include "orpheus/Command/Engine/Command.hpp"
#include "orpheus/Math/Vector.hpp"

namespace Orpheus::Command::Engine {
    class CommandMouse : public Command {
    private:
        Math::Vector2 m_coords;
        Math::Vector2 m_delta;

    public:
        CommandMouse(int x, int y, int dx, int dy) :
            m_coords(x, y),
            m_delta(dx, dy)
        {
        }

        virtual std::string getName() const override {
            return "CommandMouse";
        }

        int getX() const {
            return m_coords.getX();
        }

        int getY() const {
            return m_coords.getY();
        }

        int getDX() const {
            return m_delta.getX();
        }

        int getDY() const {
            return m_delta.getY();
        }
    };
}
