#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialFlatColor.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialText.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"

#include <GL/glew.h>

Orpheus::Render::OpenGL::Impl::Impl() {
    auto err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error("glewInit failed: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    m_materialFlatColor = std::make_shared<OpenGLImpl::Material::MaterialFlatColor>();
    m_materialText = std::make_shared<OpenGLImpl::Material::MaterialText>();
}

Orpheus::Render::OpenGL::Impl::~Impl() {
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Render::CommandClear&/* command*/) {
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
            const auto& buffer = attrib->getBuffer();
            auto it = m_buffers.find(&buffer);
            if (it == m_buffers.end()) {
                const auto& data = buffer.getData();
                unsigned int vbo;
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
                it = m_buffers.emplace(&buffer, BufferInfo{vbo, data.size()}).first;
            }

            glVertexAttribPointer(attrib->getLayout(), attrib->getElements(), GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(attrib->getLayout());

            attrib->clearCpuData();

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

    m_currentMaterial->postMaterialCommand(Command::Material::CommandPrepare());

    const auto& vertices = it->second;
    glBindVertexArray(vertices.vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.count);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>&) {
    m_currentMaterial = m_materialFlatColor;
    m_currentMaterial->use();
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Render::CommandMaterial<Orpheus::Material::MaterialText>&) {
    m_currentMaterial = m_materialText;
    m_currentMaterial->use();
}
