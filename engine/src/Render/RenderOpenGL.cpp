#include "orpheus/Render/RenderOpenGL.hpp"
#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"

Orpheus::Render::OpenGL::OpenGL(const Window::WindowPtr& window) :
    Orpheus::Render::Render(this, window),
    m_impl(std::make_unique<Impl>())
{
    addScope("OpenGL");
    Log::info(this) << "created";

    registerCommand<Command::Render::CommandClear>(*m_impl);
    registerCommand<Command::Render::CommandClearColor>(*m_impl);
    registerCommand<Command::Render::CommandVertices>(*m_impl);
    registerCommand<Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>>(*m_impl);
    registerCommand<Command::Render::CommandMaterial<Orpheus::Material::MaterialText>>(*m_impl);
    registerCommand<Command::Material::CommandColor>(*m_impl);
    registerCommand<Command::Material::CommandMatrixProjection>(*m_impl);
    registerCommand<Command::Material::CommandMatrixView>(*m_impl);
    registerCommand<Command::Material::CommandMatrixModel>(*m_impl);
    registerCommand<Command::Material::CommandText>(*m_impl);
}

Orpheus::Render::OpenGL::~OpenGL() {
}
