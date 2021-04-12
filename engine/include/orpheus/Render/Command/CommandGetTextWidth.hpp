#pragma once

#include "orpheus/Render/Command/Command.hpp"

namespace Orpheus::Render::Command {
    class GetTextWidth : public Command {
    private:
        mutable float m_res;
        float m_height;
        std::string m_text;
        std::string m_font;

    public:
        GetTextWidth(float height, const std::string& text, const std::string& font) :
            m_res(0.0f),
            m_height(height),
            m_text(text),
            m_font(font)
        {
        }

        virtual std::string getName() const override {
            return "CommandGetTextWidth";
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

        float getResult() const {
            return m_res;
        }

        void setResult(float width) const {
            m_res = width;
        }
    };
}
