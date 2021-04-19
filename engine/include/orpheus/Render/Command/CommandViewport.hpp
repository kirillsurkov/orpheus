#pragma once

#include "orpheus/Render/Command/Command.hpp"

namespace Orpheus::Render::Command {
    class Viewport : public Command {
    private:
        float m_x;
        float m_y;
        float m_width;
        float m_height;

    public:
        Viewport(float x, float y, float width, float height) :
            m_x(x), m_y(y),
            m_width(width), m_height(height)
        {
        }

        virtual std::string getName() const override {
            return "CommandViewport";
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
    };
}
