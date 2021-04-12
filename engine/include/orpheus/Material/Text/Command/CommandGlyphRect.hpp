#pragma once

#include "orpheus/Material/Command/Command.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"

namespace Orpheus::Material {
    class Text::Command::GlyphRect : public Orpheus::Material::Command::Command {
    private:
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_advance;

    public:
        GlyphRect(float x, float y, float width, float height, float advance) :
            m_x(x),
            m_y(y),
            m_width(width),
            m_height(height),
            m_advance(advance)
        {
        }

        virtual std::string getName() const override {
            return "CommandRect";
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

        float getAdvance() const {
            return m_advance;
        }
    };
}
