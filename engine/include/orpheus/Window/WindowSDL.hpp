#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/RenderOpenGL.hpp"
#include "orpheus/Utils.hpp"

#include <SDL2/SDL.h>

namespace Orpheus::WindowSDL {
    class OpenGL : public Window {
    private:
        class Context : public RenderOpenGL::Context {
        private:
            SDL_GLContext m_context;

        public:
            Context(SDL_Window* window);
            ~Context();
        };

    private:
        SDL_Window* m_window;

    public:
        static std::vector<std::string> getScopes() {
            return Utils::vectorAdd(Window::getScopes(), "SDL", "OpenGL");
        }

        OpenGL(const std::string& title, unsigned int width, unsigned int height);
        ~OpenGL();

        virtual Render::ContextPtr createContext() override;
        virtual void swapBuffers() override;
    };
};
