#include "orpheus/Window/WindowSDL.hpp"
#include "orpheus/Window/impl/WindowSDLImpl.hpp"

Orpheus::Window::SDL::SDL(const std::string& title, unsigned int width, unsigned int height) :
    m_impl(std::make_unique<Impl>(title, width, height))
{
    addScope("SDL");

    registerContextType<Render::OpenGL::ContextPtr>(this);

    try {
        m_impl->init();
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

Orpheus::Window::SDL::~SDL() {
}

void Orpheus::Window::SDL::createContext(Render::OpenGL::ContextPtr& context) {
    try {
        m_impl->createContext(context);
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

void Orpheus::Window::SDL::swapBuffers() {
    try {
        m_impl->swapBuffers();
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

void Orpheus::Window::SDL::pollEvents() {
    try {
        m_impl->pollEvents([this](const auto& command) {
            postCommand(command);
        });
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}
