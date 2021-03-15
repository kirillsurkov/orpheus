#include "orpheus/Render/RenderOpenGL.hpp"
#include <GL/gl.h>

Orpheus::Render::OpenGL::OpenGL() :
    Render(this)
{
    addScope("OpenGL");
}

Orpheus::Render::OpenGL::~OpenGL() {
}

void Orpheus::Render::OpenGL::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Orpheus::Render::OpenGL::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}
