#pragma once

#include "orpheus/Command/Command.hpp"

namespace Orpheus::Command::Render {
    class Command : public Orpheus::Command::Command {
    };

    using CommandPtr = std::shared_ptr<Command>;
}
