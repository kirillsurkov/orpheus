#pragma once

#include "orpheus/Render/Command/Command.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus::Render::Command {
    template<class T>
    class Material : public Command {
    public:
        Material() {
        }

        virtual std::string getName() const override {
            return "CommandMaterial";
        }
    };
}
