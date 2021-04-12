#pragma once

#include "orpheus/Render/Command/Command.hpp"

namespace Orpheus::Render::Command {
    class Clear : public Command {
    public:
        virtual std::string getName() const override {
            return "CommandClear";
        }
    };
}
