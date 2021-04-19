#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialFlatColor.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialText.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"

#include <GL/glew.h>

Orpheus::Render::OpenGL::Impl::Font::Font(const std::string& name, Vertex::BufferCache& bufferCache) :
    m_font(Orpheus::Font::load(name))
{
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

const Orpheus::Font& Orpheus::Render::OpenGL::Impl::Font::getFontModel() const {
    return m_font;
}

const Orpheus::Render::OpenGL::Impl::Font::Glyph& Orpheus::Render::OpenGL::Impl::Font::getGlyph(std::size_t code) const {
    auto it = m_glyphs.find(code);
    if (it == m_glyphs.end()) {
        it = m_glyphs.find('?');
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

    registerMaterial<Material::FlatColor, OpenGLImpl::Material::FlatColor>();
    registerMaterial<Material::Text,      OpenGLImpl::Material::Text>();
}

Orpheus::Render::OpenGL::Impl::~Impl() {
}

void Orpheus::Render::OpenGL::Impl::bindTexture(const Texture::Texture& texture) {
    auto it = m_textures.find(&texture);
    if (it == m_textures.end()) {
        unsigned int id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getWidth(), texture.getHeight(), 0, GL_RGBA, GL_FLOAT, texture.getCpuData().data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        m_textures.try_emplace(&texture, id);
        texture.clearCpuData();
    } else {
        glBindTexture(GL_TEXTURE_2D, it->second);
    }
}

const Orpheus::Render::OpenGL::Impl::Font& Orpheus::Render::OpenGL::Impl::getFont(const std::string& name) {
    auto it = m_fontCache.find(name);
    if (it == m_fontCache.end()) {
        it = m_fontCache.try_emplace(name, name, m_vertexBufferCache).first;
    }
    return it->second;
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::Clear&) {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::ClearColor& command) {
    const auto& color = command.getColor();
    glClearColor(color.getR(), color.getG(), color.getB(), color.getA());
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::Viewport& command) {
    m_width = command.getWidth();
    m_height = command.getHeight();
    glViewport(command.getX(), command.getY(), m_width, m_height);
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
                buffer.clearCpuData();
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
    const auto& font = getFont(command.getFont());
    const auto& fontModel = font.getFontModel();

    bindTexture(fontModel.getAtlas());
    postCommand(Material::Command::Texture(0));

    postCommand(Material::Text::Command::GlyphAppearance(command.getAppearance(), fontModel.getDistanceRange()));

    float descender = fontModel.getDescender();
    float advance = 0.0f;

    float size = 2.0f * command.getHeight() / (m_height * fontModel.getLineHeight());
    auto transform = Math::Matrix4().scale(size, size, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    std::wstring_convert<codecvt, char32_t> cvt;
    for (const auto& c : cvt.from_bytes(command.getText())) {
        const auto& glyph = font.getGlyph(c);
        const auto& glyphModel = glyph.glyph;
        const auto& worldRect = glyphModel.getWorldRect();
        const auto& atlasRect = glyphModel.getAtlasRect();

        postCommand(Material::Text::Command::GlyphModel(
                        atlasRect.width,
                        atlasRect.height,
                        transform.translate(worldRect.x + advance, worldRect.y - descender, 0.0f).scale(worldRect.width, worldRect.height, 1.0f)
                    ));
        postCommand(Orpheus::Render::Command::Vertices(glyph.vertices));

        advance += glyphModel.getAdvance();
    }
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Orpheus::Render::Command::GetTextSize& command) {
    const auto& font = getFont(command.getFont());

    float width = 0.0f;
    std::wstring_convert<codecvt, char32_t> cvt;
    for (const auto& c : cvt.from_bytes(command.getText())) {
        width += font.getGlyph(c).glyph.getAdvance();
    }

    float lineHeight = font.getFontModel().getLineHeight();
    float ratio = 2.0f * command.getTextHeight() / (m_height * lineHeight);
    command.setResult(width * ratio, lineHeight * ratio);
}
