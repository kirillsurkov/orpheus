#pragma once

#include "orpheus/Command/Render/Command.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus::Command::Render {
    template<class T>
    class CommandMaterial : public Command {
    public:
        CommandMaterial() {
        }

        virtual std::string getName() const override {
            return "CommandMaterial";
        }
    };
}
