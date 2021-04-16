#pragma once

#include "orpheus/Material/Command/Command.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"

namespace Orpheus::Material {
    class Text::Command::GlyphAppearance : public Orpheus::Material::Command::Command {
    private:
        float m_r;
        float m_g;
        float m_b;
        float m_a;
        float m_outline;

    public:
        GlyphAppearance(float r, float g, float b, float a, float outline) :
            m_r(r), m_g(g), m_b(b), m_a(a),
            m_outline(outline)
        {
        }

        virtual std::string getName() const override {
            return "CommandGlyphAppearance";
        }

        float getR() const {
            return m_r;
        }

        float getG() const {
            return m_g;
        }

        float getB() const {
            return m_b;
        }

        float getA() const {
            return m_a;
        }

        float getOutline() const {
            return m_outline;
        }

        void set(float r, float g, float b, float a, float outline) {
            m_r = r;
            m_g = g;
            m_b = b;
            m_a = a;
            m_outline = outline;
        }
    };
}
