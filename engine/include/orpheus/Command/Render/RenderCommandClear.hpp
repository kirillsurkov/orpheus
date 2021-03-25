#pragma once

#include "orpheus/Command/Render/RenderCommand.hpp"

namespace Orpheus::Command::Render {
    class RenderCommandClear : public RenderCommand {
    public:
        virtual std::string getName() const override {
            return "RenderCommandClear";
        }
    };
}
