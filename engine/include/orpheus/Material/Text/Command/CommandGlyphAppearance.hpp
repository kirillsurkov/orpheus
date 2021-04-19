#pragma once

#include "orpheus/Material/Command/Command.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"
#include "orpheus/Math/Color.hpp"

namespace Orpheus::Material {
    class Text::Command::GlyphAppearance : public Orpheus::Material::Command::Command {
    private:
        Math::Color m_outlineColor;
        float m_outline;

    public:
        GlyphAppearance(const Text::Appearance& appearance, float distanceRange) {
        }

        virtual std::string getName() const override {
            return "CommandGlyphAppearance";
        }

        const Math::Color& getOutlineColor() const {
            return m_outlineColor;
        }

        float getOutline() const {
            return m_outline;
        }
    };
}
