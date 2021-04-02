#include "orpheus/Render/RenderOpenGL.hpp"
#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"

Orpheus::Render::OpenGL::OpenGL(const Window::WindowPtr& window) :
    Orpheus::Render::Render(this, window),
    m_impl(std::make_unique<Impl>())
{
    addScope("OpenGL");
    Log::info(this) << "created";

    registerRenderCommand<Command::Render::CommandClear>(this);
    registerRenderCommand<Command::Render::CommandColor>(this);
    registerRenderCommand<Command::Render::CommandVertices>(this);
    registerRenderCommand<Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>>(this);
}

Orpheus::Render::OpenGL::~OpenGL() {
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::CommandClear>& command) {
    try {
        m_impl->onCommand(command);
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::CommandColor>& command) {
    try {
        m_impl->onCommand(command);
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::CommandVertices>& command) {
    try {
        m_impl->onCommand(command);
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>>& command) {
    try {
        m_impl->onCommand(command);
    } catch (const std::exception& e) {
        throw Exception(this, e.what());
    }
}
