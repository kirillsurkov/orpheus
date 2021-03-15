#pragma once

#include "orpheus/Render/Render.hpp"
#include "orpheus/Window/Window.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus::Render {
    class OpenGL : public Render {
    public:
        class Context : public Render::Context {};
        using ContextPtr = std::shared_ptr<Context>;

    public:
        OpenGL();
        ~OpenGL();

        virtual void setClearColor(float r, float g, float b, float a) override;
        virtual void clear() override;
    };
}
