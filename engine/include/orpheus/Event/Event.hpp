#pragma once

#include <memory>

namespace Orpheus::Event {
    class Event {
    public:
        virtual ~Event() {}
    };

    using EventPtr = std::shared_ptr<Event>;
}
