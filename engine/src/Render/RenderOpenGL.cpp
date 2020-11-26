#include "orpheus/Render/RenderOpenGL.hpp"

Orpheus::RenderOpenGL::RenderOpenGL(const Render::ContextPtr& context) :
    m_context(context)
{
    if (!std::dynamic_pointer_cast<Context>(m_context)) {
        throw std::runtime_error("Invalid context passed to RenderOpenGL");
    }
}

Orpheus::RenderOpenGL::~RenderOpenGL() {

}
