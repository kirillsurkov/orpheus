#pragma once

#include "orpheus/Material/Command/Command.hpp"

namespace Orpheus::Material::Command {
    class Prepare : public Command {
    public:
        virtual std::string getName() const override {
            return "CommandPrepare";
        }
    };
}
