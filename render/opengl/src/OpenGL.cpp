#include "render/opengl/OpenGL.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <iostream>

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
        m_random(0.0f, 1.0f)
    { }

    void OpenGL::init() {
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("glewInit() failed");
        }

        glEnable(GL_DEPTH_TEST);

        m_programFlatColor = createShader("flat_color");
        m_programGGX = createShader("ggx");
        m_programDenoise = createShader("denoise");
        m_programBlur = createShader("blur");

        std::vector<float> ltc_mat;
        std::vector<float> ltc_mag;

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

        glGenTextures(1, &m_textureLtcMat);
        glBindTexture(GL_TEXTURE_2D, m_textureLtcMat);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 64, 64, 0, GL_RGBA, GL_FLOAT, ltc_mat.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureLtcMag);
        glBindTexture(GL_TEXTURE_2D, m_textureLtcMag);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_FLOAT, ltc_mag.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureFboDepth);
        glBindTexture(GL_TEXTURE_2D, m_textureFboDepth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);

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

        glGenTextures(1, &m_textureReservoirSampleRead);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirSampleRead);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureReservoirSampleWrite);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirSampleWrite);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureReservoirWeightRead);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirWeightRead);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureReservoirWeightWrite);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirWeightWrite);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureFboDepth, 0);

        m_cube = loadMesh("cube");
        m_sphere = loadMesh("sphere");
        m_cylinder = loadMesh("cylinder");
        m_plane = loadMesh("plane");
        m_bumpy = loadMesh("bumpy");

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGL::startFrame() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, drawBuffers);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureFboColor, 0);
        clear(0.0f, 0.0f, 0.0f, 0.0f);
    }

    void OpenGL::endFrame() {
        /*glUseProgram(m_programBlur);
        {
            GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, drawBuffers);

            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureBlurPass1, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureFboNoise);
            glUniform1i(glGetUniformLocation(m_programBlur, "u_texture"), 0);
            glUniform2f(glGetUniformLocation(m_programBlur, "u_direction"), 0.0f, 1.0f);
            glUniform2f(glGetUniformLocation(m_programBlur, "u_resolution"), WIDTH, HEIGHT);
            clear(0.0f, 0.0f, 0.0f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureBlurPass2, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureBlurPass1);
            glUniform1i(glGetUniformLocation(m_programBlur, "u_texture"), 0);
            glUniform2f(glGetUniformLocation(m_programBlur, "u_direction"), 1.0f, 0.0f);
            clear(0.0f, 0.0f, 0.0f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }*/

        /*glUseProgram(m_programDenoise);
        {
            std::swap(m_textureDenoiseRead, m_textureDenoiseWrite);

            GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, drawBuffers);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureDenoiseWrite, 0);
            clear(0.0f, 0.0f, 0.0f, 1.0f);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureFboColor);
            glUniform1i(glGetUniformLocation(m_programDenoise, "u_color"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_textureFboNoise);
            glUniform1i(glGetUniformLocation(m_programDenoise, "u_noise"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_textureDenoiseRead);
            glUniform1i(glGetUniformLocation(m_programDenoise, "u_lastDenoise"), 2);

            glUniform2f(glGetUniformLocation(m_programBlur, "u_resolution"), WIDTH, HEIGHT);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }*/

        /*glUseProgram(m_programDenoise);
        {
            std::swap(m_textureDenoiseRead, m_textureDenoiseWrite);

            GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, drawBuffers);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureDenoiseWrite, 0);
            clear(0.0f, 0.0f, 0.0f, 1.0f);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureFboNoise);
            glUniform1i(glGetUniformLocation(m_programDenoise, "u_textureColor"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_textureFboNormal);
            glUniform1i(glGetUniformLocation(m_programDenoise, "u_textureNormal"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_textureDenoiseRead);
            glUniform1i(glGetUniformLocation(m_programDenoise, "u_texturePrev"), 2);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, m_textureFboColor);
            glUniform1i(glGetUniformLocation(m_programDenoise, "u_textureFboColor"), 3);

            glUniform2f(glGetUniformLocation(m_programDenoise, "u_resolution"), WIDTH, HEIGHT);
            glUniform1f(glGetUniformLocation(m_programDenoise, "u_seed"), m_random(m_randomDevice));

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }*/

        /*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);*/
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

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glReadBuffer(GL_COLOR_ATTACHMENT1);
        glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glReadBuffer(GL_DEPTH_ATTACHMENT);
        glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    }

    void OpenGL::setMaterial(const render::material::FlatColor& material) {
        glUseProgram(m_programFlatColor);

        GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, drawBuffers);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureFboColor, 0);

        glUniformMatrix4fv(glGetUniformLocation(m_programFlatColor, "u_viewProjection"), 1, GL_FALSE, &m_viewProjection.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programFlatColor, "u_modelViewProjection"), 1, GL_FALSE, &m_modelViewProjection.data[0][0]);
        glUniform3fv(glGetUniformLocation(m_programFlatColor, "u_color"), 1, material.color.data);
    }

    void OpenGL::setMaterial(const render::material::GGX& material) {
        glUseProgram(m_programGGX);

        std::swap(m_textureReservoirSampleRead, m_textureReservoirSampleWrite);
        std::swap(m_textureReservoirWeightRead, m_textureReservoirWeightWrite);

        GLenum drawBuffers[3] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(3, drawBuffers);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_textureFboNoise, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_textureReservoirSampleWrite, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, m_textureReservoirWeightWrite, 0);
        clear(0.0f, 0.0f, 0.0f, 0.0f);

        math::Matrix4x4 viewInv;
        m_math->inverse(viewInv, m_view);

        math::Matrix4x4 normalMatrix;
        m_math->inverse(normalMatrix, m_model);
        m_math->transpose(normalMatrix, normalMatrix);

        math::Vector4 origin{0.0f, 0.0f, 0.0f, 1.0f};
        m_math->mul(origin, viewInv, origin);

        glUniformMatrix4fv(glGetUniformLocation(m_programGGX, "u_model"), 1, GL_FALSE, &m_model.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programGGX, "u_normalMatrix"), 1, GL_FALSE, &normalMatrix.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_programGGX, "u_modelViewProjection"), 1, GL_FALSE, &m_modelViewProjection.data[0][0]);

        glUniform3fv(glGetUniformLocation(m_programGGX, "u_origin"), 1, origin.data);

        std::size_t lightPointsCount = material.lightPoints.size() / 3;
        std::size_t lightIndicesCount = material.lightIndices.size() / 2;
        std::size_t lightSourcesCount = lightPointsCount / 4;
        glUniform1f(glGetUniformLocation(m_programGGX, "u_roughness"), material.roughness);

        for (std::size_t i = 0; i < lightSourcesCount; i++) {
            glUniform3fv(glGetUniformLocation(m_programGGX, ("u_lightColors[" + std::to_string(i) + "]").c_str()), 1, &material.lightColors[i * 3]);
        }

        glUniform1i(glGetUniformLocation(m_programGGX, "u_lightPointsCount"), lightPointsCount);
        for (std::size_t i = 0; i < lightPointsCount; i++) {
            glUniform3fv(glGetUniformLocation(m_programGGX, ("u_lightPoints[" + std::to_string(i) + "]").c_str()), 1, &material.lightPoints[i * 3]);
        }

        glUniform1i(glGetUniformLocation(m_programGGX, "u_lightIndicesCount"), lightIndicesCount);
        for (std::size_t i = 0; i < lightIndicesCount; i++) {
            glUniform2iv(glGetUniformLocation(m_programGGX, ("u_lightIndices[" + std::to_string(i) + "]").c_str()), 1, &material.lightIndices[i * 2]);
        }

        glUniform1f(glGetUniformLocation(m_programGGX, "u_seed"), m_random(m_randomDevice));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureLtcMat);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureMat"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureLtcMag);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureMag"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirSampleRead);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureReservoir[0]"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_textureReservoirWeightRead);
        glUniform1i(glGetUniformLocation(m_programGGX, "u_textureReservoir[1]"), 3);
    }
}
