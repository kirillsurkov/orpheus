#include "orpheus/Window/WindowGLFW.hpp"
#include "orpheus/Window/impl/WindowGLFWImpl.hpp"

Orpheus::Window::GLFW::GLFW(const std::string& title, unsigned int width, unsigned int height) :
    m_impl(std::make_unique<Impl>(title, width, height))
{
    addScope("GLFW");

    registerContextType<Render::OpenGL::ContextPtr>(this);

    try {
        m_impl->init();
    }  catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

Orpheus::Window::GLFW::~GLFW() {
}

void Orpheus::Window::GLFW::swapBuffers() {
    try {
        m_impl->swapBuffers();
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

void Orpheus::Window::GLFW::createContext(Render::OpenGL::ContextPtr& context) {
    try {
        m_impl->createContext(context);
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}
