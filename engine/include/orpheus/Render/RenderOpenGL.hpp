#pragma once

#include "orpheus/Render/Render.hpp"

namespace Orpheus::Render {
    class OpenGL : public Render {
    public:
        class Context : public Render::Context {};
        using ContextPtr = std::shared_ptr<Context>;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        OpenGL(const Window::WindowPtr& window);
        ~OpenGL();
    };
}
