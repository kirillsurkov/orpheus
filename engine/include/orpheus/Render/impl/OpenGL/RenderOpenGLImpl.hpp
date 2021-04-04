#pragma once

#include "orpheus/Render/RenderOpenGL.hpp"
#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Window/Window.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus::Render {
    class OpenGL::Impl {
    public:
        class Context : public Orpheus::Render::Render::Context {};
        using ContextPtr = std::shared_ptr<Context>;

    private:
        struct Vertices {
            unsigned int vao;
            unsigned int count;
        };

        std::unordered_map<std::shared_ptr<Command::Render::CommandVertices::AttribArray>, unsigned int> m_attribs;
        std::unordered_map<const Command::Render::CommandVertices*, Vertices> m_vertices;

        OpenGLImpl::Material::MaterialPtr m_material;

        std::array<float, 4> m_color;

    public:
        Impl();
        ~Impl();

        void onCommand(const Command::Render::CommandClear& command);
        void onCommand(const Command::Render::CommandColor& command);
        void onCommand(const Command::Render::CommandVertices& command);
        void onCommand(const Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>& command);
    };
}
