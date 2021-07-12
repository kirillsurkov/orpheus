#pragma once

#include <orpheus/interface/IRender.hpp>

#include <GL/glew.h>

#include <assimp/Importer.hpp>

#include <deque>
#include <random>

namespace orpheus::render::opengl {
    class OpenGL : public orpheus::interface::IRender {
    private:
        struct Mesh {
            std::uint32_t count;
            GLuint        vao;
            GLuint        vboPositions;
            GLuint        vboNormals;
        };

        std::shared_ptr<interface::IMath> m_math;

        math::Matrix4x4 m_projection;
        math::Matrix4x4 m_view;
        math::Matrix4x4 m_model;

        math::Matrix4x4 m_viewProjection;
        math::Matrix4x4 m_modelViewProjection;

        Mesh m_cube;
        Mesh m_sphere;
        Mesh m_cylinder;
        Mesh m_plane;
        Mesh m_bumpy;

        GLuint m_textureLtcMat;
        GLuint m_textureLtcMag;

        GLuint m_fbo;
        GLuint m_textureFboDepth;
        GLuint m_textureFboColor;
        GLuint m_textureFboNormal;
        GLuint m_textureFboNoise;
        GLuint m_textureDenoiseRead;
        GLuint m_textureDenoiseWrite;
        GLuint m_textureBlurPass1;
        GLuint m_textureBlurPass2;
        GLuint m_textureReservoirSampleRead;
        GLuint m_textureReservoirWeightRead;
        GLuint m_textureReservoirSampleWrite;
        GLuint m_textureReservoirWeightWrite;

        GLuint m_programFlatColor;
        GLuint m_programGGX;
        GLuint m_programDenoise;
        GLuint m_programBlur;

        Assimp::Importer m_meshImporter;

        std::random_device m_randomDevice;
        std::uniform_real_distribution<float> m_random;

        GLuint createShader(const std::string& name);
        Mesh loadMesh(const std::string& name);

    public:
        OpenGL(const std::shared_ptr<interface::IMath>& math);

        virtual void init()                                    override;
        virtual void startFrame()                              override;
        virtual void endFrame()                                override;
        virtual void setProjection(const math::Matrix4x4& mat) override;
        virtual void setView(const math::Matrix4x4& mat)       override;
        virtual void setModel(const math::Matrix4x4& mat)      override;
        virtual void clear(float r, float g, float b, float a) override;
        virtual void drawCube()                                override;
        virtual void drawSphere()                              override;
        virtual void drawCylinder()                            override;
        virtual void drawPlane()                               override;
        virtual void drawBumpy()                               override;

        virtual void setMaterial(const render::material::FlatColor& material) override;
        virtual void setMaterial(const render::material::GGX&       material) override;
    };
}
