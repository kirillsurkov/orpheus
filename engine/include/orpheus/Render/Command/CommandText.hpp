#pragma once

#include "orpheus/Render/Command/Command.hpp"

namespace Orpheus::Render::Command {
    class Text : public Command {
    private:
        float m_height;
        std::string m_text;
        std::string m_font;

    public:
        Text(float height, const std::string& text, const std::string& font) :
            m_height(height),
            m_text(text),
            m_font(font)
        {
        }

        virtual std::string getName() const override {
            return "CommandText";
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
