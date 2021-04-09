#pragma once

#include "orpheus/Command/Render/Command.hpp"

namespace Orpheus::Command::Render {
    class CommandText : public Command {
    private:
        float m_x;
        float m_y;
        float m_height;
        std::string m_text;
        std::string m_font;

    public:
        CommandText(float x, float y, float height, const std::string& text, const std::string& font) :
            m_x(x), m_y(y),
            m_height(height),
            m_text(text),
            m_font(font)
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

        const std::string& getFont() const {
            return m_font;
        }
    };
}
