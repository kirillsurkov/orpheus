#pragma once

#include "orpheus/Render/Command/Command.hpp"

namespace Orpheus::Render::Command {
    class GetTextSize : public Command {
    private:
        float& m_resWidth;
        float& m_resHeight;
        float m_textHeight;
        std::string m_text;
        std::string m_font;

    public:
        GetTextSize(float& resWidth, float& resHeight, float textHeight, const std::string& text, const std::string& font) :
            m_resWidth(resWidth),
            m_resHeight(resHeight),
            m_textHeight(textHeight),
            m_text(text),
            m_font(font)
        {
        }

        virtual std::string getName() const override {
            return "CommandGetTextSize";
        }

        float getTextHeight() const {
            return m_textHeight;
        }

        const std::string& getText() const {
            return m_text;
        }

        const std::string& getFont() const {
            return m_font;
        }

        void setResult(float width, float height) const {
            m_resWidth = width;
            m_resHeight = height;
        }
    };
}
