#pragma once

#include "Render/Render.hpp"

class RenderOpenGL : public Render {
public:
    class Context : public Render::Context {};

private:
    Render::ContextPtr m_context;

public:
    RenderOpenGL(const Render::ContextPtr& context);
    ~RenderOpenGL();
};
