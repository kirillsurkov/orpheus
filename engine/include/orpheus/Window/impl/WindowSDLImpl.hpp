#pragma once

#include "orpheus/Window/WindowSDL.hpp"

#include <SDL2/SDL.h>

namespace Orpheus::Window {
    class SDL::Impl {
    private:
        SDL_Window* m_window;
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;

    public:
        Impl(const std::string& title, unsigned int width, unsigned int height);
        ~Impl();

        void createContext(Render::OpenGL::ContextPtr& context);

        void swapBuffers();
    };
}
