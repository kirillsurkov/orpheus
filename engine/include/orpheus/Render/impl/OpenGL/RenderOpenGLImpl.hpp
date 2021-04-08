#pragma once

#include "orpheus/Render/RenderOpenGL.hpp"
#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Command/Render/CommandClear.hpp"
#include "orpheus/Command/Render/CommandClearColor.hpp"
#include "orpheus/Command/Render/CommandVertices.hpp"
#include "orpheus/Command/Render/CommandMaterial.hpp"
#include "orpheus/Material/MaterialFlatColor.hpp"
#include "orpheus/Material/MaterialText.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"
#include "orpheus/Command/Material/CommandMatrixProjection.hpp"
#include "orpheus/Command/Material/CommandMatrixView.hpp"
#include "orpheus/Command/Material/CommandMatrixModel.hpp"
#include "orpheus/Command/Material/CommandText.hpp"

#include <type_traits>

namespace Orpheus::Render {
    class OpenGL::Impl {
    public:
        class Context : public Orpheus::Render::Render::Context {};
        using ContextPtr = std::shared_ptr<Context>;

    private:
        struct BufferInfo {
            unsigned int vbo;
            std::size_t size;
        };

        struct Vertices {
            unsigned int vao;
            unsigned int count;
        };

        std::unordered_map<const Command::Render::CommandVertices::Buffer*, BufferInfo> m_buffers;
        std::unordered_map<const Command::Render::CommandVertices*, Vertices> m_vertices;
        OpenGLImpl::Material::MaterialPtr m_materialFlatColor;
        OpenGLImpl::Material::MaterialPtr m_materialText;
        OpenGLImpl::Material::MaterialPtr m_currentMaterial;

    private:
        void onCommand(const Command::Render::CommandClear& command);
        void onCommand(const Command::Render::CommandClearColor& command);
        void onCommand(const Command::Render::CommandVertices& command);
        void onCommand(const Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>& command);
        void onCommand(const Command::Render::CommandMaterial<Orpheus::Material::MaterialText>& command);

    public:
        Impl();
        ~Impl();

        template<class T>
        void postCommand(T&& command) {
            if constexpr (std::is_base_of<Command::Material::Command, std::decay_t<T>>::value) {
                m_currentMaterial->postMaterialCommand(std::forward<T>(command));
            } else {
                onCommand(std::forward<T>(command));
            }
        }
    };
}
