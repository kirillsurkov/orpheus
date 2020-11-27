#pragma once

#include "orpheus/Log.hpp"

namespace Orpheus {
    class Application : public Loggable {
    public:
        Application();
        ~Application() = default;

        virtual void run() = 0;
    };
}
