#pragma once

#include <memory>

namespace Orpheus::Command {
    class Command {
    public:
        virtual ~Command() {}

        virtual std::string getName() const = 0;
    };
}
