#pragma once

#include "orpheus/Command/Command.hpp"

namespace Orpheus::Command {
    class CommandScenePop : public Command {
    public:
        CommandScenePop() {
        }

        virtual std::string getName() const override {
            return "CommandScenePop";
        }
    };
}
