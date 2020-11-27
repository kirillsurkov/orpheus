#pragma once

#include "orpheus/Render/Render.hpp"

namespace Orpheus::Window {
    class Window : public Loggable {
    public:
        Window() {
            addScope("Window");
        }

        virtual void swapBuffers() = 0;

        template<class T>
        void createContext(T&) {
        }
    };

    using WindowPtr = std::shared_ptr<Window>;
}
