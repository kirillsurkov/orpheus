#pragma once

#include "Render/Render.hpp"

class Window {
public:
    virtual Render::ContextPtr createContext() = 0;
    virtual void swapBuffers() = 0;
};

using WindowPtr = std::shared_ptr<Window>;
