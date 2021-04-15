#pragma once

#include "orpheus/Render/RenderOpenGL.hpp"
#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Render/Command/CommandClear.hpp"
#include "orpheus/Render/Command/CommandClearColor.hpp"
#include "orpheus/Render/Command/CommandViewport.hpp"
#include "orpheus/Render/Command/CommandVertices.hpp"
#include "orpheus/Render/Command/CommandText.hpp"
#include "orpheus/Render/Command/CommandGetTextSize.hpp"
#include "orpheus/Render/Command/CommandMaterial.hpp"
#include "orpheus/Material/FlatColor/MaterialFlatColor.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Material/Command/CommandMatrixView.hpp"
#include "orpheus/Material/Command/CommandMatrixModel.hpp"

#include "orpheus/Font.hpp"

#include <type_traits>

namespace Orpheus::Render {
    class OpenGL::Impl {
    public:
        class Context : public Orpheus::Render::Render::Context {};
        using ContextPtr = std::shared_ptr<Context>;

    private:
        struct codecvt : std::codecvt<char32_t, char, std::mbstate_t> {
            ~codecvt() {}
        };

        struct BufferInfo {
            unsigned int vbo;
            std::size_t size;
        };

        struct VerticesInfo {
            unsigned int vao;
            unsigned int count;
        };

        class Font {
        private:
            struct Glyph {
                const Orpheus::Font::Glyph& glyph;
                Vertex::Vertices vertices;

                Glyph(const Orpheus::Font::Glyph& glyph) : glyph(glyph) {}
            };

            Orpheus::Font m_font;
            std::unordered_map<std::size_t, Glyph> m_glyphs;
            unsigned int m_textureId;

        public:
            Font(const std::string& name, Vertex::BufferCache& bufferCache);
            ~Font();

            const Orpheus::Font& getFont() const;
            unsigned int getTextureId() const;
            const Glyph& getGlyph(std::size_t code) const;
        };

        Vertex::BufferCache& m_vertexBufferCache;
        std::unordered_map<const Vertex::Buffer*, BufferInfo> m_buffers;
        std::unordered_map<const Vertex::Vertices*, VerticesInfo> m_vertices;
        std::unordered_map<std::string, Font> m_fontCache;
        TypeDispatcher m_materialDispatcher;
        OpenGLImpl::Material::MaterialPtr m_currentMaterial;
        OpenGLImpl::Material::MaterialPtr m_materialFlatColor;
        OpenGLImpl::Material::MaterialPtr m_materialText;
        float m_width;
        float m_height;

    private:
        void onCommand(const Orpheus::Render::Command::Clear& command);
        void onCommand(const Orpheus::Render::Command::ClearColor& command);
        void onCommand(const Orpheus::Render::Command::Viewport& command);
        void onCommand(const Orpheus::Render::Command::Vertices& command);
        void onCommand(const Orpheus::Render::Command::Text& command);
        void onCommand(const Orpheus::Render::Command::GetTextSize& command);

        template<class T>
        void onCommand(const Orpheus::Render::Command::Material<T>&) {
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
        Impl(Caches& caches);
        ~Impl();

        template<class T>
        void postCommand(T&& command) {
            if constexpr (std::is_base_of<Material::Command::Command, std::decay_t<T>>::value) {
                m_currentMaterial->postCommand(std::forward<T>(command));
            } else {
                onCommand(std::forward<T>(command));
            }
        }
    };
}
