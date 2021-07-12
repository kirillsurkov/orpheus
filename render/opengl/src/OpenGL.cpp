#include "render/opengl/OpenGL.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

namespace orpheus::render::opengl {
    namespace {
        const char* vss = "#version 330 core\n"
                          "uniform mat4 u_viewProjection;\n"
                          "uniform mat4 u_modelViewProjection;\n"
                          "layout(location = 0) in vec3 aPosition;"
                          "layout(location = 1) in vec3 aNormal;"
                          "out vec3 vNormal;"
                          "void main() {\n"
                          "    vNormal = aNormal;"
                          "    gl_Position = u_modelViewProjection * vec4(aPosition, 1.0);"
                          "}";

        const char* fss = "#version 330 core\n"
                          "in vec3 vNormal;"
                          "out vec4 outColor;"
                          "void main() {\n"
                          "    outColor = vec4(abs(vNormal.xyz), 1);"
                          "}";
    }

    OpenGL::Mesh OpenGL::loadMesh(const std::string& name) {
        const aiScene* scene = m_meshImporter.ReadFile("./res/models/" + name + "/geometry.obj", aiProcess_Triangulate);

        std::vector<float> positions;
        std::vector<float> normals;

        for (unsigned int meshId = 0; meshId < scene->mNumMeshes; meshId++) {
            aiMesh* mesh = scene->mMeshes[meshId];
            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                const auto& face = mesh->mFaces[i];
                for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
                    const auto& pos = mesh->mVertices[face.mIndices[j]];
                    positions.push_back(pos.x);
                    positions.push_back(pos.y);
                    positions.push_back(pos.z);

                    const auto& normal = mesh->mNormals[face.mIndices[j]];
                    normals.push_back(normal.x);
                    normals.push_back(normal.y);
                    normals.push_back(normal.z);
                }
            }
        }

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vboPositions;
        glGenBuffers(1, &vboPositions);
        glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        GLuint vboNormals;
        glGenBuffers(1, &vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        return Mesh{static_cast<std::uint32_t>(positions.size() / 3), vao, vboPositions, vboNormals};
    }

    OpenGL::OpenGL(const std::shared_ptr<interface::IMath>& math) :
        m_math(math)
    { }

    void OpenGL::init() {
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("glewInit() failed");
        }

        char buffer[1024];
        GLsizei len;

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vss, nullptr);
        glCompileShader(vs);
        glGetShaderInfoLog(vs, sizeof(buffer), &len, buffer);
        std::cout << buffer << std::endl;

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fss, nullptr);
        glCompileShader(fs);
        glGetShaderInfoLog(fs, sizeof(buffer), &len, buffer);
        std::cout << buffer << std::endl;

        m_prog = glCreateProgram();
        glAttachShader(m_prog, vs);
        glAttachShader(m_prog, fs);
        glLinkProgram(m_prog);

        glEnable(GL_DEPTH_TEST);

        m_cube = loadMesh("cube");
        m_sphere = loadMesh("sphere");
    }

    void OpenGL::setProjection(const interface::math::Matrix4x4& mat) {
        m_projection = mat;

        m_viewProjection = m_projection;
        m_math->mul(m_viewProjection, m_view);

        m_modelViewProjection = m_viewProjection;
        m_math->mul(m_modelViewProjection, m_model);
    }

    void OpenGL::setView(const interface::math::Matrix4x4& mat) {
        m_view = mat;

        m_viewProjection = m_projection;
        m_math->mul(m_viewProjection, m_view);

        m_modelViewProjection = m_viewProjection;
        m_math->mul(m_modelViewProjection, m_model);
    }

    void OpenGL::setModel(const interface::math::Matrix4x4& mat) {
        m_model = mat;

        m_modelViewProjection = m_viewProjection;
        m_math->mul(m_modelViewProjection, m_model);
    }

    void OpenGL::clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL::drawCube() {
        glUseProgram(m_prog);

        glUniformMatrix4fv(glGetUniformLocation(m_prog, "u_viewProjection"), 1, GL_FALSE, &m_viewProjection.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_prog, "u_modelViewProjection"), 1, GL_FALSE, &m_modelViewProjection.data[0][0]);

        glBindVertexArray(m_cube.vao);
        glDrawArrays(GL_TRIANGLES, 0, m_cube.count);
    }

    void OpenGL::drawSphere() {
        glUseProgram(m_prog);

        glUniformMatrix4fv(glGetUniformLocation(m_prog, "u_viewProjection"), 1, GL_FALSE, &m_viewProjection.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_prog, "u_modelViewProjection"), 1, GL_FALSE, &m_modelViewProjection.data[0][0]);

        glBindVertexArray(m_sphere.vao);
        glDrawArrays(GL_TRIANGLES, 0, m_sphere.count);
    }
}
