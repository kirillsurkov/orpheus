#include "orpheus/Render/RenderOpenGL.hpp"

Orpheus::Render::OpenGL::OpenGL(const Window::WindowPtr& window) {
    addScope("OpenGL");
    window->createContext(m_context);
}

Orpheus::Render::OpenGL::~OpenGL() {
}
