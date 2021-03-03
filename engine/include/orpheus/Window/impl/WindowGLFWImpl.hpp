#pragma once

#include "orpheus/Window/WindowGLFW.hpp"

#include <GLFW/glfw3.h>

namespace Orpheus::Window {
    class GLFW::Impl {
    private:
        GLFWwindow* m_window;
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;

    public:
        Impl(const std::string& title, unsigned int width, unsigned int height);
        ~Impl();

        void init();
        void createContext(Render::OpenGL::ContextPtr& context);
        void swapBuffers();
    };
}
