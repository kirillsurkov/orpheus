#include "render/opengl/OpenGL.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

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
        m_math(math),
        m_randomFloat(0.0f, 1.0f)
    { }

    void OpenGL::init() {
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("glewInit() failed");
        }

        m_programFlatColor = createShader("flat_color");
        m_programGGX = createShader("ggx");
        m_programDenoise = createShader("denoise");
        m_programBlur = createShader("blur");

        std::vector<float> ltc_mat;
        std::vector<float> ltc_mag;
        std::vector<unsigned char> noise;

        std::vector<unsigned char> floorColor;
        std::vector<unsigned char> floorNormal;
        std::vector<unsigned char> floorRoughness;

        {
            std::ifstream input("./res/ltc_mat.dds");
            float f;
            while (input >> f) ltc_mat.push_back(f);
        }

        {
            std::ifstream input("./res/ltc_mag.dds");
            float f;
            while (input >> f) ltc_mag.push_back(f);
        }

        {
            std::ifstream input("./res/noise.dds");
            int c;
            while (input >> c) noise.push_back(c);
        }

        {
            std::ifstream input("./res/floor_color.ppm");
            int c;
            while (input >> c) floorColor.push_back(c);
        }

        {
            std::ifstream input("./res/floor_normal.ppm");
            int c;
            while (input >> c) floorNormal.push_back(c);
        }

        {
            std::ifstream input("./res/floor_roughness.ppm");
            int c;
            while (input >> c) floorRoughness.push_back(c);
        }

        glGenTextures(1, &m_textureLtcMat);
        glBindTexture(GL_TEXTURE_2D, m_textureLtcMat);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 64, 64, 0, GL_RGBA, GL_FLOAT, ltc_mat.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureLtcMag);
        glBindTexture(GL_TEXTURE_2D, m_textureLtcMag);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, ltc_mag.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureNoise);
        glBindTexture(GL_TEXTURE_2D, m_textureNoise);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, noise.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenTextures(1, &m_textureFloorColor);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, floorColor.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureFloorNormal);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, floorNormal.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureFloorRoughness);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorRoughness);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, floorRoughness.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureFboDepthColor);
        glBindTexture(GL_TEXTURE_2D, m_textureFboDepthColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureFboDepthNoise);
        glBindTexture(GL_TEXTURE_2D, m_textureFboDepthNoise);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureFboColor);
        glBindTexture(GL_TEXTURE_2D, m_textureFboColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureFboNormal);
        glBindTexture(GL_TEXTURE_2D, m_textureFboNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureFboNoise);
        glBindTexture(GL_TEXTURE_2D, m_textureFboNoise);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureDenoiseRead);
        glBindTexture(GL_TEXTURE_2D, m_textureDenoiseRead);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureDenoiseWrite);
        glBindTexture(GL_TEXTURE_2D, m_textureDenoiseWrite);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureBlurPass1);
        glBindTexture(GL_TEXTURE_2D, m_textureBlurPass1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureBlurPass2);
        glBindTexture(GL_TEXTURE_2D, m_textureBlurPass2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureReservoirRead);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirRead);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureReservoirWrite);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirWrite);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenFramebuffers(1, &m_fbo);

        m_cube = loadMesh("cube");
        m_sphere = loadMesh("sphere");
        m_cylinder = loadMesh("cylinder");
        m_plane = loadMesh("plane");
        m_bumpy = loadMesh("bumpy");
    }

    void OpenGL::startFrame() {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WIDTH, HEIGHT);
        clear(0.0f, 0.0f, 0.0f, 0.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glViewport(0, 0, WIDTH, HEIGHT);
        GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, drawBuffers);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureFboColor, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_textureFboNoise, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureFboDepthColor, 0);
        clear(0.0f, 0.0f, 0.0f, 0.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureFboDepthNoise, 0);
        clear(0.0f, 0.0f, 0.0f, 0.0f);

        std::swap(m_textureReservoirRead, m_textureReservoirWrite);
    }

    void OpenGL::endFrame() {
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WIDTH, HEIGHT);

        clear(0.0f, 0.0f, 0.0f, 0.0f);

        glUseProgram(m_programBlur);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureFboColor);
        glUniform1i(glGetUniformLocation(m_programBlur, "u_texture1"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureFboDepthColor);
        glUniform1i(glGetUniformLocation(m_programBlur, "u_textureDepth1"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureFboNoise);
        glUniform1i(glGetUniformLocation(m_programBlur, "u_texture2"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_textureFboDepthNoise);
        glUniform1i(glGetUniformLocation(m_programBlur, "u_textureDepth2"), 3);

        glDrawArrays(GL_TRIANGLES, 0, 6);
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
        m_math->mul(m_viewProjection, m_projection, m_view);
        m_math->mul(m_modelViewProjection, m_viewProjection, m_model);
        m_math->inverse(m_viewInv, m_view);
    }

    void OpenGL::setModel(const math::Matrix4x4& mat) {
        m_model = mat;
        m_math->mul(m_modelViewProjection, m_viewProjection, m_model);
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

        GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, drawBuffers);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureFboDepthColor, 0);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureFboColor, 0);

        glUniformMatrix4fv(glGetUniformLocation(m_programFlatColor, "u_viewProjection"), 1, GL_FALSE, &m_viewProjection.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programFlatColor, "u_modelViewProjection"), 1, GL_FALSE, &m_modelViewProjection.data[0][0]);
        glUniform3fv(glGetUniformLocation(m_programFlatColor, "u_color"), 1, material.color.data);
    }

    void OpenGL::setMaterial(const render::material::GGX& material) {
        glUseProgram(m_programGGX);

        GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(2, drawBuffers);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureFboDepthNoise, 0);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_textureFboNoise, 0);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_textureReservoirWrite, 0);

        math::Matrix4x4 normalMatrix;
        m_math->inverse(normalMatrix, m_model);
        m_math->transpose(normalMatrix, normalMatrix);

        math::Vector4 origin{0.0f, 0.0f, 0.0f, 1.0f};
        m_math->mul(origin, m_viewInv, origin);

        glUniformMatrix4fv(glGetUniformLocation(m_programGGX, "u_model"), 1, GL_FALSE, &m_model.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programGGX, "u_normalMatrix"), 1, GL_FALSE, &normalMatrix.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programGGX, "u_modelViewProjection"), 1, GL_FALSE, &m_modelViewProjection.data[0][0]);

        glUniform3fv(glGetUniformLocation(m_programGGX, "u_origin"), 1, origin.data);

        glUniform1f(glGetUniformLocation(m_programGGX, "u_roughness"), material.roughness);

        setSSBO(material.lightsBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_ssboMap[material.lightsBuffer]);

        glUniform1f(glGetUniformLocation(m_programGGX, "u_seed"), m_randomFloat(m_randomDevice));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureLtcMat);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureMat"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureLtcMag);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureMag"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureNoise);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureNoise"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirRead);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureReservoir"), 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorColor);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureFloorColor"), 4);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorNormal);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureFloorNormal"), 5);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, m_textureFloorRoughness);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureFloorRoughness"), 6);
    }
}
