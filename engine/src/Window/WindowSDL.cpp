#include "orpheus/Window/WindowSDL.hpp"
#include "orpheus/Log.hpp"

Orpheus::WindowSDL::OpenGL::Context::Context(SDL_Window* window) {
    m_context = SDL_GL_CreateContext(window);
    if (m_context == nullptr) {
        throw std::runtime_error("SDL_GL_CreateContext failed");
    }
}

Orpheus::WindowSDL::OpenGL::Context::~Context() {
    SDL_GL_DeleteContext(m_context);
}

Orpheus::WindowSDL::OpenGL::OpenGL(const std::string& title, unsigned int width, unsigned int height) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (m_window == nullptr) {
        throw std::runtime_error("SDL_CreateWindow failed");
    }

    Log::info(this) << "test";
}

Orpheus::WindowSDL::OpenGL::~OpenGL() {
    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}

Orpheus::Render::ContextPtr Orpheus::WindowSDL::OpenGL::createContext() {
    return std::make_shared<Context>(m_window);
}

void Orpheus::WindowSDL::OpenGL::swapBuffers() {
    SDL_GL_SwapWindow(m_window);
}
