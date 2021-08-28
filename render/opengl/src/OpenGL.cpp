#include "render/opengl/OpenGL.hpp"

#include "render/opengl/PNGReader.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <iostream>
#include <chrono>

#define WIDTH 1600
#define HEIGHT 900

namespace orpheus::render::opengl {
    GLuint OpenGL::createShader(const std::string& name) {
        std::vector<char> vss;
        std::vector<char> fss;

        {
            std::ifstream input("./res/shaders/" + name + "/vertex.glsl");
            input.seekg(0, std::ios::end);
            vss.resize(input.tellg());
            input.seekg(0, std::ios::beg);
            input.read(vss.data(), vss.size());
            vss.push_back(0);
        }

        {
            std::ifstream input("./res/shaders/" + name + "/fragment.glsl");
            input.seekg(0, std::ios::end);
            fss.resize(input.tellg());
            input.seekg(0, std::ios::beg);
            input.read(fss.data(), fss.size());
            fss.push_back(0);
        }

        const char* vssData = vss.data();
        const char* fssData = fss.data();

        char buffer[1024];
        GLsizei len;

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vssData, nullptr);
        glCompileShader(vs);
        glGetShaderInfoLog(vs, sizeof(buffer), &len, buffer);
        if (len > 0) std::cout << name << " vertex: " << buffer << std::endl;

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fssData, nullptr);
        glCompileShader(fs);
        glGetShaderInfoLog(fs, sizeof(buffer), &len, buffer);
        if (len > 0) std::cout << name << " fragment: " << buffer << std::endl;

        GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);

        return prog;
    }

    OpenGL::Mesh OpenGL::loadMesh(const std::string& name) {
        const aiScene* scene = m_meshImporter.ReadFile("./res/models/" + name + "/geometry.obj", aiProcess_Triangulate);

        std::vector<float> positions;
        std::vector<float> normals;

        for (unsigned int meshId = 0; meshId < scene->mNumMeshes; meshId++) {
            aiMesh* mesh = scene->mMeshes[meshId];
            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                const auto& face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    auto index = face.mIndices[j];

                    const auto& pos = mesh->mVertices[index];
                    positions.insert(positions.end(), {pos.x, pos.y, pos.z});

                    const auto& normal = mesh->mNormals[index];
                    normals.insert(normals.end(), {normal.x, normal.y, normal.z});
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

    GLuint OpenGL::createTexture(GLint internalFormat, std::uint32_t width, std::uint32_t height, const void* data, GLenum type, GLint format) {
        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat == GL_DEPTH_COMPONENT32F ? GL_DEPTH_COMPONENT : format, type, data);
        return id;
    }

    GLuint OpenGL::loadTexture(const std::string& name, GLint internalFormat) {
        std::uint32_t width;
        std::uint32_t height;
        std::vector<unsigned char> pixels;
        PNGReader::read("./res/textures/" + name, width, height, pixels);
        return createTexture(internalFormat, width, height, pixels.data());
    }

    OpenGL::OpenGL(const std::shared_ptr<interface::IMath>& math) :
        m_math(math),
        m_jitter({1.0f, 1.0f}),
        m_randomFloat(0.0f, 1.0f)
    { }

    void OpenGL::init() {
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("glewInit() failed");
        }

        m_cube = loadMesh("cube");
        m_sphere = loadMesh("sphere");
        m_cylinder = loadMesh("cylinder");
        m_plane = loadMesh("plane");
        m_bumpy = loadMesh("bumpy");

        m_programFlatColor = createShader("flat_color");
        m_programBRDF = createShader("brdf");
        m_programBRDF_GBuffer = createShader("brdf_gbuffer");
        m_programCombine = createShader("combine");

        m_textureNoise = loadTexture("noise/noise.png", GL_RGBA32F);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        m_textureFloorColor = loadTexture("floor/color.png", GL_RGB32F);
        m_textureFloorNormal = loadTexture("floor/normal.png", GL_RGB32F);
        m_textureFloorRoughness = loadTexture("floor/roughness.png", GL_R32F);

        {
            std::vector<float> data;
            std::ifstream input("./res/textures/ltc/ggx/ltc_1");
            float f;
            while (input >> f) data.push_back(f);
            m_textureBRDF_GGX_ltc1 = createTexture(GL_RGBA32F, 64, 64, data.data(), GL_FLOAT);
        }

        {
            std::vector<float> data;
            std::ifstream input("./res/textures/ltc/ggx/ltc_2");
            float f;
            while (input >> f) data.push_back(f);
            m_textureBRDF_GGX_ltc2 = createTexture(GL_RGBA32F, 64, 64, data.data(), GL_FLOAT);
        }

        m_textureReservoirRead = createTexture(GL_RGB32F, WIDTH, HEIGHT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        m_textureReservoirWrite = createTexture(GL_RGB32F, WIDTH, HEIGHT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        m_textureFboDepth = createTexture(GL_DEPTH_COMPONENT32F, WIDTH, HEIGHT);
        m_textureFboColor = createTexture(GL_RGB, WIDTH, HEIGHT);

        m_textureFboBRDFDepth = createTexture(GL_DEPTH_COMPONENT32F, WIDTH, HEIGHT);
        m_textureFboBRDFColor = createTexture(GL_RGB, WIDTH, HEIGHT);
        m_textureFboBRDFPosition = createTexture(GL_RGB32F, WIDTH, HEIGHT);
        m_textureFboBRDFNormal = createTexture(GL_RGB32F, WIDTH, HEIGHT);
        m_textureFboBRDFRoughness = createTexture(GL_RED, WIDTH, HEIGHT);
        m_textureFboBRDFMotion = createTexture(GL_RG32F, WIDTH, HEIGHT);
        m_textureFboBRDFResult = createTexture(GL_RGB, WIDTH, HEIGHT);

        {
            glGenFramebuffers(1, &m_fboFlatColor);
            glBindFramebuffer(GL_FRAMEBUFFER, m_fboFlatColor);
            glViewport(0, 0, WIDTH, HEIGHT);
            GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, drawBuffers);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureFboDepth, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureFboColor, 0);
        }

        {
            glGenFramebuffers(1, &m_fboBRDF_GBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, m_fboBRDF_GBuffer);
            glViewport(0, 0, WIDTH, HEIGHT);
            GLenum drawBuffers[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
            glDrawBuffers(5, drawBuffers);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureFboBRDFDepth, 0);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureFboBRDFColor, 0);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_textureFboBRDFPosition, 0);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_textureFboBRDFNormal, 0);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, m_textureFboBRDFRoughness, 0);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, m_textureFboBRDFMotion, 0);
        }

        {
            glGenFramebuffers(1, &m_fboBRDF);
            glBindFramebuffer(GL_FRAMEBUFFER, m_fboBRDF);
            glViewport(0, 0, WIDTH, HEIGHT);
            GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
            glDrawBuffers(2, drawBuffers);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureFboBRDFDepth, 0);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureFboBRDFResult, 0);
        }
    }

    void OpenGL::startFrame() {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WIDTH, HEIGHT);
        clear(0.0f, 0.0f, 0.0f, 0.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboFlatColor);
        clear(0.0f, 0.0f, 0.0f, 0.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboBRDF_GBuffer);
        clear(0.0f, 0.0f, 0.0f, 0.0f);
    }

    void OpenGL::stageBRDF() {
        glUseProgram(m_programBRDF);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboBRDF);

        math::Vector4 origin{0.0f, 0.0f, 0.0f, 1.0f};
        m_math->mul(origin, m_viewInv, origin);
        glUniform3fv(glGetUniformLocation(m_programBRDF, "u_origin"), 1, origin.data);

        glUniform3fv(glGetUniformLocation(m_programBRDF, "u_forward"), 1, m_forward.data);
        glUniform3fv(glGetUniformLocation(m_programBRDF, "u_right"), 1, m_right.data);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureBRDF_GGX_ltc1);
        glUniform1i(glGetUniformLocation(m_programBRDF, "u_textureLtc1"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureBRDF_GGX_ltc2);
        glUniform1i(glGetUniformLocation(m_programBRDF, "u_textureLtc2"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureNoise);
        glUniform1i(glGetUniformLocation(m_programBRDF, "u_textureNoise"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_textureFboBRDFColor);
        glUniform1i(glGetUniformLocation(m_programBRDF, "u_textureColor"), 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_textureFboBRDFPosition);
        glUniform1i(glGetUniformLocation(m_programBRDF, "u_texturePosition"), 4);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_textureFboBRDFNormal);
        glUniform1i(glGetUniformLocation(m_programBRDF, "u_textureNormal"), 5);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, m_textureFboBRDFRoughness);
        glUniform1i(glGetUniformLocation(m_programBRDF, "u_textureRoughness"), 6);

        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, m_textureFboBRDFMotion);
        glUniform1i(glGetUniformLocation(m_programBRDF, "u_textureMotion"), 7);

        for (int i = 0; i < 4; i++) {
            std::swap(m_textureReservoirRead, m_textureReservoirWrite);

            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_textureReservoirWrite, 0);

            glUniform1f(glGetUniformLocation(m_programBRDF, "u_seed"), m_randomFloat(m_randomDevice));
            glUniform1i(glGetUniformLocation(m_programBRDF, "u_useMotion"), i == 0);

            glActiveTexture(GL_TEXTURE8);
            glBindTexture(GL_TEXTURE_2D, m_textureReservoirRead);
            glUniform1i(glGetUniformLocation(m_programBRDF, "u_textureReservoir"), 8);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    void OpenGL::stageCombine() {
        glUseProgram(m_programCombine);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureFboColor);
        glUniform1i(glGetUniformLocation(m_programCombine, "u_texture1"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureFboDepth);
        glUniform1i(glGetUniformLocation(m_programCombine, "u_textureDepth1"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureFboBRDFResult);
        glUniform1i(glGetUniformLocation(m_programCombine, "u_texture2"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_textureFboBRDFDepth);
        glUniform1i(glGetUniformLocation(m_programCombine, "u_textureDepth2"), 3);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void OpenGL::endFrame() {
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        stageBRDF();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WIDTH, HEIGHT);
        stageCombine();

        m_jitter.x() = -m_jitter.x();
        m_jitter.y() = -m_jitter.y();
    }

    void OpenGL::setSSBO(render::SsboId ssbo) {
        auto it = m_ssboMap.find(ssbo);
        if (it == m_ssboMap.end()) {
            GLuint id;
            glGenBuffers(1, &id);
            it = m_ssboMap.emplace(ssbo, id).first;
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, it->second);
    }

    void* OpenGL::ssboMapBuffer() {
        return glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    }

    void OpenGL::ssboUnmapBuffer() {
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }

    void OpenGL::ssboSetSize(std::uint32_t size) {
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_COPY);
    }

    void OpenGL::setProjection(const math::Matrix4x4& mat) {
        m_projection = mat;
        m_math->mul(m_viewProjection, m_projection, m_view);
        m_math->mul(m_modelViewProjection, m_viewProjection, m_model);
    }

    void OpenGL::setView(const math::Matrix4x4& mat) {
        m_view = mat;
        m_prevViewProjection = m_viewProjection;
        m_math->mul(m_viewProjection, m_projection, m_view);
        m_math->mul(m_modelViewProjection, m_viewProjection, m_model);
        m_math->inverse(m_viewInv, m_view);
    }

    void OpenGL::setModel(const math::Matrix4x4& mat) {
        m_model = mat;
        m_math->mul(m_modelViewProjection, m_viewProjection, m_model);
    }

    void OpenGL::setPrevModel(const math::Matrix4x4& mat) {
        m_prevModel = mat;
    }

    void OpenGL::setForward(const math::Vector3& vec) {
        m_forward = vec;
    }

    void OpenGL::setRight(const math::Vector3& vec) {
        m_right = vec;
    }

    void OpenGL::clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL::drawCube() {
        glBindVertexArray(m_cube.vao);
        glDrawArrays(GL_TRIANGLES, 0, m_cube.count);
    }

    void OpenGL::drawSphere() {
        glBindVertexArray(m_sphere.vao);
        glDrawArrays(GL_TRIANGLES, 0, m_sphere.count);
    }

    void OpenGL::drawCylinder() {
        glBindVertexArray(m_cylinder.vao);
        glDrawArrays(GL_TRIANGLES, 0, m_cylinder.count);
    }

    void OpenGL::drawPlane() {
        glBindVertexArray(m_plane.vao);
        glDrawArrays(GL_TRIANGLES, 0, m_plane.count);
    }

    void OpenGL::drawBumpy() {
        glBindVertexArray(m_bumpy.vao);
        glDrawArrays(GL_TRIANGLES, 0, m_bumpy.count);
    }

    void OpenGL::setMaterial(const render::material::FlatColor& material) {
        glUseProgram(m_programFlatColor);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboFlatColor);

        glUniformMatrix4fv(glGetUniformLocation(m_programFlatColor, "u_viewProjection"), 1, GL_FALSE, &m_viewProjection.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programFlatColor, "u_modelViewProjection"), 1, GL_FALSE, &m_modelViewProjection.data[0][0]);
        glUniform3fv(glGetUniformLocation(m_programFlatColor, "u_color"), 1, material.color.data);
    }

    void OpenGL::setMaterial(const render::material::GGX& material) {
        glUseProgram(m_programBRDF_GBuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboBRDF_GBuffer);

        math::Matrix4x4 normalMatrix;
        m_math->inverse(normalMatrix, m_model);
        m_math->transpose(normalMatrix, normalMatrix);

        math::Matrix4x4 prevMVP = m_prevViewProjection;
        m_math->mul(prevMVP, prevMVP, m_prevModel);

        glUniformMatrix4fv(glGetUniformLocation(m_programBRDF_GBuffer, "u_model"), 1, GL_FALSE, &m_model.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programBRDF_GBuffer, "u_normalMatrix"), 1, GL_FALSE, &normalMatrix.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programBRDF_GBuffer, "u_modelViewProjection"), 1, GL_FALSE, &m_modelViewProjection.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programBRDF_GBuffer, "u_prevModelViewProjection"), 1, GL_FALSE, &prevMVP.data[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorColor);
        glUniform1i(glGetUniformLocation(m_programBRDF_GBuffer, "u_textureFloorColor"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorNormal);
        glUniform1i(glGetUniformLocation(m_programBRDF_GBuffer, "u_textureFloorNormal"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorRoughness);
        glUniform1i(glGetUniformLocation(m_programBRDF_GBuffer, "u_textureFloorRoughness"), 2);

        setSSBO(material.lightsBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_ssboMap[material.lightsBuffer]);
    }
}
