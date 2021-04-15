#include "orpheus/Render/RenderOpenGL.hpp"
#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"

Orpheus::Render::OpenGL::OpenGL(Caches& caches, Window::Window& window) :
    Orpheus::Render::Render(this, window),
    m_impl(std::make_unique<Impl>(caches))
{
    addScope("OpenGL");
    Log::info(this) << "created";

    registerCommand<Orpheus::Render::Command::Clear>(*m_impl);
    registerCommand<Orpheus::Render::Command::ClearColor>(*m_impl);
    registerCommand<Orpheus::Render::Command::Viewport>(*m_impl);
    registerCommand<Orpheus::Render::Command::Vertices>(*m_impl);
    registerCommand<Orpheus::Render::Command::Text>(*m_impl);
    registerCommand<Orpheus::Render::Command::GetTextSize>(*m_impl);
    registerCommand<Orpheus::Render::Command::Material<Orpheus::Material::FlatColor>>(*m_impl);
    registerCommand<Orpheus::Render::Command::Material<Orpheus::Material::Text>>(*m_impl);
    registerCommand<Material::Command::Color>(*m_impl);
    registerCommand<Material::Command::MatrixProjection>(*m_impl);
    registerCommand<Material::Command::MatrixView>(*m_impl);
    registerCommand<Material::Command::MatrixModel>(*m_impl);
}

Orpheus::Render::OpenGL::~OpenGL() {
}
