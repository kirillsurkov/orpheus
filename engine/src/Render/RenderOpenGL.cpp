#include "orpheus/Render/RenderOpenGL.hpp"
#include <GL/glew.h>

Orpheus::Render::OpenGL::OpenGL() :
    Render(this)
{
    addScope("OpenGL");

    registerRenderCommand<Command::Render::CommandClear>(this);
    registerRenderCommand<Command::Render::CommandSetClearColor>(this);
    registerRenderCommand<Command::Render::CommandVertices>(this);
}

Orpheus::Render::OpenGL::~OpenGL() {
}

std::string getShaderInfo(unsigned int shader) {
    std::size_t bufferSize = 2048;
    std::vector<char> buffer(bufferSize);
    int len;
    glGetShaderInfoLog(shader, bufferSize, &len, buffer.data());
    return std::string(buffer.data(), len);
}

void Orpheus::Render::OpenGL::onCreate() {
    Log::info(this) << "created";

    auto err = glewInit();
    if (err != GLEW_OK) {
        throw Exception(this, "glewInit failed: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    auto vss = "#version 330 core\n"
               "layout(location = 0) in vec2 position;\n"
               "void main() {\n"
               "    gl_Position = vec4(position, 0.0, 1.0);\n"
               "}";
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vss, nullptr);
    glCompileShader(vs);
    Log::info(this) << getShaderInfo(vs);

    auto fss = "#version 330 core\n"
               "out vec4 color;\n"
               "void main() {\n"
               "    color = vec4(1.0, 1.0, 1.0, 1.0);\n"
               "}";
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fss, nullptr);
    glCompileShader(fs);
    Log::info(this) << getShaderInfo(fs);

    auto prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glUseProgram(prog);
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::CommandClear>&/* command*/) {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::CommandSetClearColor>& command) {
    glClearColor(command->getR(), command->getG(), command->getB(), command->getA());
}

void Orpheus::Render::OpenGL::onCommand(const std::shared_ptr<Command::Render::CommandVertices>& command) {
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
                throw Exception(this, "VertexAttrib different lengths");
            }
        }

        it = m_vertices.emplace(command, Vertices{vao, verticesCount}).first;
    }

    const auto& vertices = it->second;
    glBindVertexArray(vertices.vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.count);
}
