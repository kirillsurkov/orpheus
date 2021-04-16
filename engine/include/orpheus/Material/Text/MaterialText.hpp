#pragma once

#include "orpheus/Material/Material.hpp"
#include "orpheus/Material/Command/Command.hpp"

namespace Orpheus::Material {
    class Text : public Material {
    public:
        struct Command;

    public:
        virtual std::string getName() const override {
            return "MaterialText";
        }
    };

    struct Text::Command {
        class GlyphModel;
        class GlyphAppearance;
    };
}
