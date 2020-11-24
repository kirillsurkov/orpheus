#pragma once

#include "Render/RenderOpenGL.hpp"
#include "Window/Window.hpp"

#include <SDL2/SDL.h>

namespace WindowSDL {
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
        OpenGL(const std::string& title, unsigned int width, unsigned int height);
        ~OpenGL();

        virtual Render::ContextPtr createContext() override;
        virtual void swapBuffers() override;
    };
};
