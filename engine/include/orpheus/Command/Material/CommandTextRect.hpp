#pragma once

#include "orpheus/Command/Material/Command.hpp"

namespace Orpheus::Command::Material::Text {
    class CommandRect : public Command {
    private:
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_advance;

    public:
        CommandRect(float x, float y, float width, float height, float advance) :
            m_x(x),
            m_y(y),
            m_width(width),
            m_height(height),
            m_advance(advance)
        {
        }

        virtual std::string getName() const override {
            return "CommandRectSize";
        }

        float getX() const {
            return m_x;
        }

        float getY() const {
            return m_y;
        }

        float getWidth() const {
            return m_width;
        }

        float getHeight() const {
            return m_height;
        }

        float getAdvance() const {
            return m_advance;
        }
    };
}
