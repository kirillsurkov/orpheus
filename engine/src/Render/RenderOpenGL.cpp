#include "orpheus/Render/RenderOpenGL.hpp"

Orpheus::Render::OpenGL::OpenGL(const Window::WindowPtr& window) {
    addScope("OpenGL");
    try {
        window->createContext(m_context);
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

Orpheus::Render::OpenGL::~OpenGL() {
}
