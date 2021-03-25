#include "orpheus/Render/RenderOpenGL.hpp"
#include <GL/gl.h>

Orpheus::Render::OpenGL::OpenGL() :
    Render(this)
{
    addScope("OpenGL");

    registerRenderCommand<Command::Render::RenderCommandClear>(this);
    registerRenderCommand<Command::Render::RenderCommandSetClearColor>(this);
}

Orpheus::Render::OpenGL::~OpenGL() {
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::RenderCommandClear>&/* command*/) {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::RenderCommandSetClearColor>& command) {
    glClearColor(command->getR(), command->getG(), command->getB(), command->getA());
}
