#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialFlatColor.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"

#include <GL/glew.h>

Orpheus::Render::OpenGL::Impl::Impl() {
    auto err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error("glewInit failed: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    m_material = std::make_shared<OpenGLImpl::Material::MaterialFlatColor>();
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
            const auto& array = attrib->getArray();

            unsigned int vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, array.size() * sizeof(float), array.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(attrib->getLayout(), attrib->getElements(), GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(attrib->getLayout());

            m_attribs.emplace(attrib, vbo);
            attrib->clearCpuData();

            if (first) {
                verticesCount = array.size() / attrib->getElements();
                first = false;
            }

            if (verticesCount != array.size() / attrib->getElements()) {
                throw std::runtime_error("VertexAttrib different lengths");
            }
        }

        it = m_vertices.emplace(&command, Vertices{vao, verticesCount}).first;
    }

    m_material->postMaterialCommand(Command::Material::CommandPrepare());

    const auto& vertices = it->second;
    glBindVertexArray(vertices.vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.count);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>&/* command*/) {
    m_material->use();
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Material::CommandColor& command) {
    m_material->postMaterialCommand(command);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Material::CommandMatrixProjection& command) {
    m_material->postMaterialCommand(command);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Material::CommandMatrixView& command) {
    m_material->postMaterialCommand(command);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const Command::Material::CommandMatrixModel& command) {
    m_material->postMaterialCommand(command);
}
