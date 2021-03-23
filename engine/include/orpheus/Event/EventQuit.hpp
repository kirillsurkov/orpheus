#pragma once

#include "orpheus/Event/Event.hpp"

namespace Orpheus::Event {
    class EventQuit : public Event {
    public:
        virtual std::string getName() const override {
            return "EventQuit";
        }
    };
}