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
        math::Matrix4x4 m_viewInv;
        math::Matrix4x4 m_model;
        math::Matrix4x4 m_prevModel;

        math::Matrix4x4 m_viewProjection;
        math::Matrix4x4 m_prevViewProjection;
        math::Matrix4x4 m_modelViewProjection;

        math::Vector3   m_forward;
        math::Vector3   m_right;

        math::Vector2   m_jitter;

        Mesh m_cube;
        Mesh m_sphere;
        Mesh m_cylinder;
        Mesh m_plane;
        Mesh m_bumpy;

        GLuint m_textureNoise;

        GLuint m_textureBRDF_GGX_ltc1;
        GLuint m_textureBRDF_GGX_ltc2;

        GLuint m_textureFloorColor;
        GLuint m_textureFloorNormal;
        GLuint m_textureFloorRoughness;

        GLuint m_fboFlatColor;
        GLuint m_textureFboDepth;
        GLuint m_textureFboColor;
        GLuint m_textureReservoirRead;
        GLuint m_textureReservoirWrite;

        GLuint m_fboBRDF_GBuffer;
        GLuint m_textureFboBRDFDepth;
        GLuint m_textureFboBRDFColor;
        GLuint m_textureFboBRDFPosition;
        GLuint m_textureFboBRDFNormal;
        GLuint m_textureFboBRDFRoughness;
        GLuint m_textureFboBRDFMotion;

        GLuint m_fboBRDF;
        GLuint m_textureFboBRDFResult;

        GLuint m_fboCombine;
        GLuint m_textureFboCombineResult;

        GLuint m_programFlatColor;
        GLuint m_programBRDF;
        GLuint m_programBRDF_GBuffer;
        GLuint m_programCombine;
        GLuint m_programBlit;

        Assimp::Importer m_meshImporter;

        std::random_device                    m_randomDevice;
        std::uniform_real_distribution<float> m_randomFloat;

        std::unordered_map<SsboId, GLuint> m_ssboMap;

        GLuint createShader(const std::string& name);

        Mesh loadMesh(const std::string& name);

        GLuint createTexture(GLint internalFormat, std::uint32_t width, std::uint32_t height, const void* data = nullptr, GLenum type = GL_UNSIGNED_BYTE, GLint format = GL_RGBA);
        GLuint loadTexture(const std::string& name, GLint internalFormat);

        void stageBRDF();
        void stageCombine(GLuint tex1, GLuint depth1, GLuint tex2, GLuint depth2);
        void stageBlit(GLuint tex);

    public:
        OpenGL(const std::shared_ptr<interface::IMath>& math);

        virtual void init()                                    override;
        virtual void startFrame()                              override;
        virtual void endFrame()                                override;

        virtual void  setSSBO(render::SsboId ssbo)             override;
        virtual void* ssboMapBuffer()                          override;
        virtual void  ssboUnmapBuffer()                        override;
        virtual void  ssboSetSize(std::uint32_t size)          override;

        virtual void setProjection(const math::Matrix4x4& mat) override;
        virtual void setView(const math::Matrix4x4& mat)       override;
        virtual void setModel(const math::Matrix4x4& mat)      override;
        virtual void setPrevModel(const math::Matrix4x4& mat)  override;

        virtual void setForward(const math::Vector3& vec)      override;
        virtual void setRight(const math::Vector3& vec)        override;

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
