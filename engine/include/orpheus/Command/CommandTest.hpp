#pragma once

#include "orpheus/Command/Command.hpp"

namespace Orpheus::Command {
    class CommandTest : public Command {
    private:
        std::string m_message;

    public:
        CommandTest(const std::string& message) :
            m_message(message)
        {
        }

        virtual std::string getName() const override {
            return "CommandTest";
        }

        const std::string& getMessage() const {
            return m_message;
        }
    };
}
