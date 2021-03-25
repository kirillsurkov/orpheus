#pragma once

#include "orpheus/Command/Command.hpp"

namespace Orpheus::Command::Render {
    class RenderCommand : public Command {
    };

    using RenderCommandPtr = std::shared_ptr<RenderCommand>;
}
