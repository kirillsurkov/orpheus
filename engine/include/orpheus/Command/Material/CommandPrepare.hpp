#pragma once

#include "orpheus/Command/Material/Command.hpp"

namespace Orpheus::Command::Material {
    class CommandPrepare : public Command {
    public:
        virtual std::string getName() const override {
            return "CommandPrepare";
        }
    };
}
