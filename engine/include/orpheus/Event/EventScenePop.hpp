#pragma once

#include "orpheus/Event/Event.hpp"

namespace Orpheus::Event {
    class EventScenePop : public Event {
    public:
        EventScenePop() {
        }

        virtual std::string getName() const override {
            return "EventScenePop";
        }
    };
}
