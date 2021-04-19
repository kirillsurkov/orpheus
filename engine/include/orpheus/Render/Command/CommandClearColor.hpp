#pragma once

#include "orpheus/Render/Command/Command.hpp"
#include "orpheus/Math/Color.hpp"

namespace Orpheus::Render::Command {
    class ClearColor : public Command {
    private:
        Math::Color m_color;

    public:
        ClearColor(float r, float g, float b, float a) :
            m_color(r, g, b, a)
        {
        }

        virtual std::string getName() const override {
            return "CommandClearColor";
        }

        Math::Color& getColor() {
            return m_color;
        }

        const Math::Color& getColor() const {
            return m_color;
        }
    };
}
