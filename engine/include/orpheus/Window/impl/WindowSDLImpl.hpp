#pragma once

#include "orpheus/Window/WindowSDL.hpp"

#include <SDL2/SDL.h>

namespace Orpheus::Window {
    class SDL::Impl {
    private:
        class Context : public Render::OpenGL::Context {
        private:
            SDL_GLContext m_context;

        public:
            Context(SDL_Window* window) {
                m_context = SDL_GL_CreateContext(window);
                if (m_context == nullptr) {
                    throw std::runtime_error("SDL_GL_CreateContext failed");
                }
            }

            ~Context() {
                SDL_GL_DeleteContext(m_context);
            }
        };

    private:
        SDL_Window* m_window;
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
