#pragma once

#include "orpheus/Render/Render.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus {
    class RenderOpenGL : public Render {
    public:
        class Context : public Render::Context {};

    private:
        Render::ContextPtr m_context;

    public:
        static std::vector<std::string> getScopes() {
            return Utils::vectorAdd(Render::getScopes(), "OpenGL");
        }

        RenderOpenGL(const Render::ContextPtr& context);
        ~RenderOpenGL();
    };
}
