#pragma once

#include "orpheus/Event/Event.hpp"

namespace Orpheus::Event {
    class EventTest : public Event {
    private:
        std::string m_message;

    public:
        EventTest(const std::string& message) :
            m_message(message)
        {
        }

        virtual std::string getName() const override {
            return "EventTest";
        }

        const std::string& getMessage() const {
            return m_message;
        }
    };
}
