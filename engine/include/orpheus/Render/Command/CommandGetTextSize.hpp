#pragma once

#include "orpheus/Render/Command/Command.hpp"
#include "orpheus/Math/Vector.hpp"

namespace Orpheus::Render::Command {
    class GetTextSize : public Command {
    private:
        Math::Vector2& m_res;
        float m_textHeight;
        std::string m_text;
        std::string m_font;

    public:
        GetTextSize(Math::Vector2& res, float textHeight, const std::string& text, const std::string& font) :
            m_res(res),
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
            m_res.set(width, height);
        }
    };
}
