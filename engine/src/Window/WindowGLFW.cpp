#include "orpheus/Window/WindowGLFW.hpp"
#include "orpheus/Exception.hpp"

Orpheus::Window::GLFW::GLFW(const std::string& title, unsigned int width, unsigned int height) {
    addScope("GLFW");
}

Orpheus::Window::GLFW::~GLFW() {
}

void Orpheus::Window::GLFW::swapBuffers() {
    throw Orpheus::Exception(this, "not implemented");
}
