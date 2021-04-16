#pragma once

#include "orpheus/Render/Command/Command.hpp"
#include "orpheus/Material/Text/Command/CommandGlyphAppearance.hpp"

namespace Orpheus::Render::Command {
    class Text : public Command {
    private:
        float m_height;
        std::string m_text;
        std::string m_font;
        Orpheus::Material::Text::Command::GlyphAppearance m_appearance;

    public:
        Text(float height, const std::string& text, const std::string& font, const Orpheus::Material::Text::Command::GlyphAppearance& appearance) :
            m_height(height),
            m_text(text),
            m_font(font),
            m_appearance(appearance)
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

        const Orpheus::Material::Text::Command::GlyphAppearance& getAppearance() const {
            return m_appearance;
        }
    };
}
