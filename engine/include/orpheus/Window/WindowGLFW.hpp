#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus::WindowGLFW {
    class OpenGL : public Window {
    private:
        class Context {
        public:
            Context();
            ~Context();
        };

    public:
        static std::vector<std::string> getScopes() {
            return Utils::vectorAdd(Window::getScopes(), "GLFW", "OpenGL");
        }

        OpenGL(const std::string& title, unsigned int width, unsigned int height);
        ~OpenGL();

        virtual Render::ContextPtr createContext() override;
        virtual void swapBuffers() override;
    };
};
