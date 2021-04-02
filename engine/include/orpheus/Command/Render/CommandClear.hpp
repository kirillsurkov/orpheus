#pragma once

#include "orpheus/Command/Render/Command.hpp"

namespace Orpheus::Command::Render {
    class CommandClear : public Command {
    public:
        virtual std::string getName() const override {
            return "CommandClear";
        }
    };
}
