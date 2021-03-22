#pragma once

#include "orpheus/Event/Event.hpp"

namespace Orpheus::Event {
    template<class T>
    class EventLoadScene : public Event {
    public:
        EventLoadScene() {
        }

        virtual std::string getName() const override {
            return "EventLoadScene";
        }
    };
}
