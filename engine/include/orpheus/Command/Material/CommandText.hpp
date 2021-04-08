#pragma once

#include "orpheus/Command/Material/Command.hpp"

namespace Orpheus::Command::Material {
    class CommandText : public Command {
    private:
        float m_x;
        float m_y;
        float m_height;
        std::string m_text;

    public:
        CommandText(float x, float y, float height, const std::string& text) :
            m_x(x), m_y(y),
            m_height(height),
            m_text(text)
        {
        }

        virtual std::string getName() const override {
            return "CommandText";
        }

        float getX() const {
            return m_x;
        }

        float getY() const {
            return m_y;
        }

        float getHeight() const {
            return m_height;
        }

        const std::string& getText() const {
            return m_text;
        }
    };
}
