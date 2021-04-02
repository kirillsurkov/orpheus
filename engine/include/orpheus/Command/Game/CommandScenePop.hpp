#pragma once

#include "orpheus/Command/Game/Command.hpp"

namespace Orpheus::Command::Game {
    class CommandScenePop : public Command {
    public:
        virtual std::string getName() const override {
            return "CommandScenePop";
        }
    };
}
