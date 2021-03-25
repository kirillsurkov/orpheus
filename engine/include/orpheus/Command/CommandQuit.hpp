#pragma once

#include "orpheus/Command/Command.hpp"

namespace Orpheus::Command {
    class CommandQuit : public Command {
    public:
        virtual std::string getName() const override {
            return "CommandQuit";
        }
    };
}
