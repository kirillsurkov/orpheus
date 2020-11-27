#include "orpheus/Window/WindowSDL.hpp"
#include "orpheus/Log.hpp"
#include "orpheus/Window/impl/WindowSDLImpl.hpp"

Orpheus::Window::SDL::SDL(const std::string& title, unsigned int width, unsigned int height) :
    m_impl(std::make_unique<Impl>(title, width, height))
{
    addScope("SDL");
}

void Orpheus::Window::SDL::createContext(Render::OpenGL::ContextPtr& context) {
    try {
        m_impl->createContext(context);
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

void Orpheus::Window::SDL::swapBuffers() {
    m_impl->swapBuffers();
}
