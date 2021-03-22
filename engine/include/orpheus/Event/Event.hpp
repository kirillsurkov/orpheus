#pragma once

#include <memory>

namespace Orpheus::Event {
    class Event {
    public:
        virtual ~Event() {}

        virtual std::string getName() const = 0;
    };

    using EventPtr = std::shared_ptr<Event>;
}
