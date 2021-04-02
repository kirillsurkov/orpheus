#include "orpheus/Render/impl/OpenGL/RenderOpenGLImpl.hpp"
#include "orpheus/Render/impl/OpenGL/Material/MaterialFlatColor.hpp"
#include <GL/glew.h>

Orpheus::Render::OpenGL::Impl::Impl() {
    m_color = {0, 0, 0, 0};

    auto err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error("glewInit failed: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    m_material = std::make_shared<OpenGLImpl::Material::MaterialFlatColor>();
}

Orpheus::Render::OpenGL::Impl::~Impl() {
}

void Orpheus::Render::OpenGL::Impl::onCommand(const std::shared_ptr<Command::Render::CommandClear>&/* command*/) {
    glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const std::shared_ptr<Command::Render::CommandColor>& command) {
    m_color = {command->getR(), command->getG(), command->getB(), command->getA()};
    m_material->postMaterialCommand(command);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const std::shared_ptr<Command::Render::CommandVertices>& command) {
    auto it = m_vertices.find(command);
    if (it == m_vertices.end()) {
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        bool first = true;
        unsigned int verticesCount;

        for (const auto& attrib : command->getAttribs()) {
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

        it = m_vertices.emplace(command, Vertices{vao, verticesCount}).first;
    }

    const auto& vertices = it->second;
    glBindVertexArray(vertices.vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.count);
}

void Orpheus::Render::OpenGL::Impl::onCommand(const std::shared_ptr<Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>>&/* command*/) {
    m_material->use();
}
