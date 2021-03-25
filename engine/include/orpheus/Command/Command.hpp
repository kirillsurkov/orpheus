#pragma once

#include <memory>

namespace Orpheus::Command {
    class Command {
    public:
        virtual ~Command() {}

        virtual std::string getName() const = 0;
    };

    using CommandPtr = std::shared_ptr<Command>;
}
