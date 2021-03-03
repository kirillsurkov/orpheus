#include "orpheus/Window/impl/WindowGLFWImpl.hpp"

Orpheus::Window::GLFW::Impl::Impl(const std::string& title, unsigned int width, unsigned int height) :
    m_window(nullptr),
    m_title(title),
    m_width(width),
    m_height(height)
{
}

Orpheus::Window::GLFW::Impl::~Impl() {
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Orpheus::Window::GLFW::Impl::init() {
    if (!glfwInit()) {
        throw std::runtime_error("glfwInit failed");
    }
}

void Orpheus::Window::GLFW::Impl::createContext(Render::OpenGL::ContextPtr& context) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) {
        throw std::runtime_error("glfwCreateWindow failed");
    }
}

void Orpheus::Window::GLFW::Impl::swapBuffers() {
    glfwSwapBuffers(m_window);
}
