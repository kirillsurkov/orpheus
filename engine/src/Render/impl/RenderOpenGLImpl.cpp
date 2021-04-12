#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialFlatColor.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialText.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Orpheus::Render::OpenGL::Impl::Font::Font(const std::string& name, Vertex::BufferCache& bufferCache) :
    m_font(Orpheus::Font::load(name))
{
    const auto& atlas = m_font.getAtlas();
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas.getWidth(), atlas.getHeight(), 0, GL_RGBA, GL_FLOAT, atlas.getData().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    const auto& bufferRect = bufferCache.get("rect_left_bottom");

    for (const auto& pair : m_font.getGlyphs()) {
        auto& glyph = m_glyphs.emplace(pair.first, pair.second).first->second;
        glyph.vertices.addAttrib(0, bufferRect);
        auto& bufferUV = bufferCache.add("font_" + name + "_uv_" + std::to_string(pair.first), 2);
        const auto& atlasRect = pair.second.getAtlasRect();
        bufferUV.addPoint(atlasRect.x,                   atlasRect.y);
        bufferUV.addPoint(atlasRect.x + atlasRect.width, atlasRect.y);
        bufferUV.addPoint(atlasRect.x + atlasRect.width, atlasRect.y + atlasRect.height);
        bufferUV.addPoint(atlasRect.x + atlasRect.width, atlasRect.y + atlasRect.height);
        bufferUV.addPoint(atlasRect.x,                   atlasRect.y);
        bufferUV.addPoint(atlasRect.x,                   atlasRect.y + atlasRect.height);
        glyph.vertices.addAttrib(1, bufferUV);
    }
}

Orpheus::Render::OpenGL::Impl::Font::~Font() {
    glDeleteTextures(1, &m_textureId);
}

const Orpheus::Font& Orpheus::Render::OpenGL::Impl::Font::getFont() const {
    return m_font;
}

unsigned int Orpheus::Render::OpenGL::Impl::Font::getTextureId() const {
    return m_textureId;
}

const Orpheus::Render::OpenGL::Impl::Font::Glyph& Orpheus::Render::OpenGL::Impl::Font::getGlyph(std::size_t code) const {
    auto it = m_glyphs.find(code);
    if (it == m_glyphs.end()) {
        throw std::runtime_error("Glyph not found");
    }
    return it->second;
}

Orpheus::Render::OpenGL::Impl::Impl(Caches& caches) :
    m_vertexBufferCache(caches.vertexBufferCache())
{
    auto err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error("glewInit failed: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    m_materialFlatColor = std::make_shared<OpenGLImpl::Material::FlatColor>();
    m_materialText = std::make_shared<OpenGLImpl::Material::Text>();

    registerMaterial<Material::FlatColor>(m_materialFlatColor);
    registerMaterial<Material::Text>(m_materialText);
}

Orpheus::Render::OpenGL::Impl::~Impl() {
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::Clear&) {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::ClearColor& command) {
    glClearColor(command.getR(), command.getG(), command.getB(), command.getA());
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::Vertices& command) {
    const auto& vertices = command.getVertices();
    auto it = m_vertices.find(&vertices);
    if (it == m_vertices.end()) {
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        bool first = true;
        unsigned int verticesCount;

        for (const auto& attrib : vertices.getAttribs()) {
            const auto& buffer = attrib.getBuffer();
            auto it = m_buffers.find(&buffer);
            if (it == m_buffers.end()) {
                const auto& data = buffer.getData();
                unsigned int vbo;
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
                it = m_buffers.emplace(&buffer, BufferInfo{vbo, data.size()}).first;
                //attrib.clearCpuData();
            }

            glBindBuffer(GL_ARRAY_BUFFER, it->second.vbo);
            glVertexAttribPointer(attrib.getLayout(), buffer.getElements(), GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(attrib.getLayout());

            if (first) {
                verticesCount = it->second.size / buffer.getElements();
                first = false;
            }

            if (verticesCount != it->second.size / buffer.getElements()) {
                throw std::runtime_error("VertexAttrib different lengths");
            }
        }

        it = m_vertices.emplace(&vertices, VerticesInfo{vao, verticesCount}).first;
    }

    m_currentMaterial->postCommand(Material::Command::Prepare());

    const auto& info = it->second;
    glBindVertexArray(info.vao);
    glDrawArrays(GL_TRIANGLES, 0, info.count);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::Text& command) {
    const auto& fontName = command.getFont();
    auto it = m_fontCache.find(fontName);
    if (it == m_fontCache.end()) {
        it = m_fontCache.try_emplace(fontName, fontName, m_vertexBufferCache).first;
    }

    const auto& font = it->second;
    const auto& textureId = it->second.getTextureId();

    postCommand(Material::Command::Texture(textureId));

    float advance = 0.0f;

    for (const auto& c : command.getText()) {
        const auto& glyph = font.getGlyph(c);
        const auto& glyphModel = glyph.glyph;
        const auto& worldRect = glyphModel.getWorldRect();

        postCommand(Material::Text::Command::GlyphRect(worldRect.x, worldRect.y, worldRect.width, worldRect.height, advance));
        postCommand(Orpheus::Render::Command::Vertices(glyph.vertices));

        advance += glyphModel.getAdvance();
    }
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::GetTextWidth& command) {
    const auto& fontName = command.getFont();
    auto it = m_fontCache.find(fontName);
    if (it == m_fontCache.end()) {
        it = m_fontCache.try_emplace(fontName, fontName, m_vertexBufferCache).first;
    }

    const auto& font = it->second;

    float width = 0.0f;
    for (const auto& c : command.getText()) {
        width += font.getGlyph(c).glyph.getAdvance();
    }

    command.setResult(width);
}
