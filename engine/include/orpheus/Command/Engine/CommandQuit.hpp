#pragma once

#include "orpheus/Command/Engine/Command.hpp"

namespace Orpheus::Command::Engine {
    class CommandQuit : public Command {
    public:
        virtual std::string getName() const override {
            return "CommandQuit";
        }
    };
}
