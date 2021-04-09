#pragma once

#include "orpheus/Render/RenderOpenGL.hpp"
#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Command/Render/CommandClear.hpp"
#include "orpheus/Command/Render/CommandClearColor.hpp"
#include "orpheus/Command/Render/CommandVertices.hpp"
#include "orpheus/Command/Render/CommandText.hpp"
#include "orpheus/Command/Render/CommandMaterial.hpp"
#include "orpheus/Material/MaterialFlatColor.hpp"
#include "orpheus/Material/MaterialText.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"
#include "orpheus/Command/Material/CommandMatrixProjection.hpp"
#include "orpheus/Command/Material/CommandMatrixView.hpp"
#include "orpheus/Command/Material/CommandMatrixModel.hpp"

#include "orpheus/Font.hpp"

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

        class Font {
        private:
            struct Glyph {
                const Orpheus::Font::Glyph& glyph;
                Command::Render::CommandVertices command;

                Glyph(const Orpheus::Font::Glyph& glyph) : glyph(glyph) {}
            };

            Orpheus::Font m_font;
            std::unordered_map<std::size_t, Glyph> m_glyphs;
            unsigned int m_textureId;

        public:
            Font(const std::string& name, const std::shared_ptr<Command::Render::CommandVertices::AttribArray>& rectVertices);
            ~Font();

            const Orpheus::Font& getFont() const;
            unsigned int getTextureId() const;
            const Glyph& getGlyph(std::size_t code) const;
        };

        std::unordered_map<const Command::Render::CommandVertices::AttribArray*, BufferInfo> m_buffers;
        std::unordered_map<const Command::Render::CommandVertices*, Vertices> m_vertices;
        std::unordered_map<std::string, Font> m_fontCache;
        TypeDispatcher m_materialDispatcher;
        OpenGLImpl::Material::MaterialPtr m_currentMaterial;
        OpenGLImpl::Material::MaterialPtr m_materialFlatColor;
        OpenGLImpl::Material::MaterialPtr m_materialText;

        Command::Render::CommandVertices m_verticesRect;

    private:
        void onCommand(const Command::Render::CommandClear& command);
        void onCommand(const Command::Render::CommandClearColor& command);
        void onCommand(const Command::Render::CommandVertices& command);
        void onCommand(const Command::Render::CommandText& command);

        template<class T>
        void onCommand(const Command::Render::CommandMaterial<T>&) {
            m_materialDispatcher.dispatch(Utils::TypeIdentity<T>{});
        }

        template<class T, class U>
        void registerMaterial(U&& material) {
            m_materialDispatcher.registerType<Utils::TypeIdentity<T>>([this, material](auto&&) {
                m_currentMaterial = material;
                m_currentMaterial->use();
            });
        }

    public:
        Impl();
        ~Impl();

        template<class T>
        void postCommand(T&& command) {
            if constexpr (std::is_base_of<Command::Material::Command, std::decay_t<T>>::value) {
                m_currentMaterial->postCommand(std::forward<T>(command));
            } else {
                onCommand(std::forward<T>(command));
            }
        }
    };
}
