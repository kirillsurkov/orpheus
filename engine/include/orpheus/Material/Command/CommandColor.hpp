#pragma once

#include "orpheus/Material/Command/Command.hpp"
#include "orpheus/Math/Color.hpp"

namespace Orpheus::Material::Command {
    class Color : public Command {
    private:
        Math::Color m_color;

    public:
        Color(float r, float g, float b, float a) :
            m_color(r, g, b, a)
        {
        }

        Color(const Math::Color& color) :
            Color(color.getR(), color.getG(), color.getB(), color.getA())
        {
        }

        virtual std::string getName() const override {
            return "CommandColor";
        }

        Math::Color& getColor() {
            return m_color;
        }

        const Math::Color& getColor() const {
            return m_color;
        }
    };
}
