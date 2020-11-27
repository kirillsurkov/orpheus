#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus::Window {
    class GLFW : public Window {
    public:
        GLFW(const std::string& title, unsigned int width, unsigned int height);
        ~GLFW();

        //createContext(Render::Render::ContextPtr& context);

        virtual void swapBuffers() override;
    };
};
