#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialFlatColor.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialText.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Orpheus::Render::OpenGL::Impl::Font::Font(const std::string& name, const std::shared_ptr<Command::Render::CommandVertices::AttribArray>& rectVertices) :
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

    for (const auto& pair : m_font.getGlyphs()) {
        auto& glyph = m_glyphs.emplace(pair.first, pair.second).first->second;
        glyph.command.addAttrib(rectVertices);
        auto& uv = glyph.command.addAttrib(1, 2);
        const auto& atlasRect = pair.second.getAtlasRect();
        uv->addPoint(atlasRect.x,                   atlasRect.y);
        uv->addPoint(atlasRect.x + atlasRect.width, atlasRect.y);
        uv->addPoint(atlasRect.x + atlasRect.width, atlasRect.y + atlasRect.height);
        uv->addPoint(atlasRect.x + atlasRect.width, atlasRect.y + atlasRect.height);
        uv->addPoint(atlasRect.x,                   atlasRect.y);
        uv->addPoint(atlasRect.x,                   atlasRect.y + atlasRect.height);
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

Orpheus::Render::OpenGL::Impl::Impl() {
    auto err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error("glewInit failed: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    m_materialFlatColor = std::make_shared<OpenGLImpl::Material::MaterialFlatColor>();
    m_materialText = std::make_shared<OpenGLImpl::Material::MaterialText>();

    registerMaterial<Material::MaterialFlatColor>(m_materialFlatColor);
    registerMaterial<Material::MaterialText>(m_materialText);

    auto& positions = m_verticesRect.addAttrib(0, 2);
    positions->addPoint(0.0f, 0.0f);
    positions->addPoint(1.0f, 0.0f);
    positions->addPoint(1.0f, 1.0f);
    positions->addPoint(1.0f, 1.0f);
    positions->addPoint(0.0f, 0.0f);
    positions->addPoint(0.0f, 1.0f);
}

Orpheus::Render::OpenGL::Impl::~Impl() {
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Render::CommandClear&) {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Render::CommandClearColor& command) {
    glClearColor(command.getR(), command.getG(), command.getB(), command.getA());
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Render::CommandVertices& command) {
    auto it = m_vertices.find(&command);
    if (it == m_vertices.end()) {
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        bool first = true;
        unsigned int verticesCount;

        for (const auto& attrib : command.getAttribs()) {
            auto it = m_buffers.find(attrib.get());
            if (it == m_buffers.end()) {
                const auto& buffer = attrib->getBuffer();
                const auto& data = buffer.getData();
                unsigned int vbo;
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
                it = m_buffers.emplace(attrib.get(), BufferInfo{vbo, data.size()}).first;
                attrib->clearCpuData();
            }

            glBindBuffer(GL_ARRAY_BUFFER, it->second.vbo);
            glVertexAttribPointer(attrib->getLayout(), attrib->getElements(), GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(attrib->getLayout());

            if (first) {
                verticesCount = it->second.size / attrib->getElements();
                first = false;
            }

            if (verticesCount != it->second.size / attrib->getElements()) {
                throw std::runtime_error("VertexAttrib different lengths");
            }
        }

        it = m_vertices.emplace(&command, Vertices{vao, verticesCount}).first;
    }

    m_currentMaterial->postCommand(Command::Material::CommandPrepare());

    const auto& vertices = it->second;
    glBindVertexArray(vertices.vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.count);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Render::CommandText& command) {
    const auto& fontName = command.getFont();
    auto it = m_fontCache.find(fontName);
    if (it == m_fontCache.end()) {
        it = m_fontCache.try_emplace(fontName, fontName, m_verticesRect.getAttribs().front()).first;
    }

    const auto& font = it->second;
    const auto& textureId = it->second.getTextureId();

    postCommand(Command::Material::CommandTexture(textureId));

    float advance = 0.0f;

    for (const auto& c : command.getText()) {
        const auto& glyph = font.getGlyph(c);
        const auto& glyphModel = glyph.glyph;
        const auto& worldRect = glyphModel.getWorldRect();

        postCommand(Command::Material::Text::CommandRect(worldRect.x, worldRect.y, worldRect.width, worldRect.height, advance));
        postCommand(glyph.command);

        advance += glyphModel.getAdvance();// + worldRect.x;
    }
}
