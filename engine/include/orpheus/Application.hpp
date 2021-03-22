#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"

namespace Orpheus {
    class Application : public Loggable {
    public:
        Application();
        ~Application() = default;

        void run();

        virtual bool step() = 0;
    };
}
