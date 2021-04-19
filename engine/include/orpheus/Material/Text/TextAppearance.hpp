#pragma once

#include "orpheus/Material/Command/Command.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"
#include "orpheus/Math/Color.hpp"

namespace Orpheus::Material {
    class Text::Appearance {
    private:
        Math::Color m_outlineColor;
        float m_outline;

    public:
        const Math::Color& getOutlineColor() {
            return m_outlineColor;
        }

        float getOutline() const {
            return m_outline;
        }
    };
}
