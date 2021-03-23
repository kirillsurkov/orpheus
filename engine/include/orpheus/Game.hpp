#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"

namespace Orpheus {
    class Game : public Loggable {
    public:
        Game();
        ~Game() = default;

        void run();

        virtual bool step() = 0;
    };
}
