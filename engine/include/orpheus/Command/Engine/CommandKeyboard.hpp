#pragma once

#include "orpheus/Command/Engine/Command.hpp"
#include "orpheus/Keys.hpp"

namespace Orpheus::Command::Engine {
    class CommandKeyboard : public Command {
    private:
        Input::Key m_key;
        bool m_down;

    public:
        CommandKeyboard(Input::Key key, bool down) :
            m_key(key),
            m_down(down)
        {
        }

        Input::Key getKey() const {
            return m_key;
        }

        bool isDown() const {
            return m_down;
        }

        virtual std::string getName() const override {
            return "CommandKeyboard";
        }
    };
}
