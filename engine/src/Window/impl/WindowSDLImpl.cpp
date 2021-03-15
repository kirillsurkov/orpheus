#include "orpheus/Window/impl/WindowSDLImpl.hpp"

Orpheus::Window::SDL::Impl::Impl(const std::string& title, unsigned int width, unsigned int height) :
    m_window(nullptr),
    m_title(title),
    m_width(width),
    m_height(height)
{
}

Orpheus::Window::SDL::Impl::~Impl() {
    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}

void Orpheus::Window::SDL::Impl::init() {
    int err = SDL_Init(SDL_INIT_VIDEO);
    if (err != 0) {
        throw std::runtime_error("SDL_Init failed with error = " + std::to_string(err));
    }
}

void Orpheus::Window::SDL::Impl::createContext(Render::OpenGL::ContextPtr& context) {
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

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (m_window == nullptr) {
        throw std::runtime_error("SDL_CreateWindow failed");
    }

    context = std::make_shared<Context>(m_window);
}

void Orpheus::Window::SDL::Impl::swapBuffers() {
    SDL_GL_SwapWindow(m_window);
}
