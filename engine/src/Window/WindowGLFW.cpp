#include "orpheus/Window/WindowGLFW.hpp"

Orpheus::WindowGLFW::OpenGL::OpenGL(const std::string& title, unsigned int width, unsigned int height) {
    throw std::runtime_error("WindowGLFW::OpenGL not implemented");
}

Orpheus::WindowGLFW::OpenGL::~OpenGL() {
}

Orpheus::Render::ContextPtr Orpheus::WindowGLFW::OpenGL::createContext() {
    throw std::runtime_error("WindowGLFW::OpenGL::createContext not implemented");
}

void Orpheus::WindowGLFW::OpenGL::swapBuffers() {
    throw std::runtime_error("WindowGLFW::OpenGL::swapBuffers not implemented");
}
