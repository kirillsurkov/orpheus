#pragma once

#include <orpheus/interface/IRender.hpp>

#include <GL/glew.h>

#include <assimp/Importer.hpp>

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

        interface::math::Matrix4x4 m_projection;
        interface::math::Matrix4x4 m_view;
        interface::math::Matrix4x4 m_model;

        interface::math::Matrix4x4 m_viewProjection;
        interface::math::Matrix4x4 m_modelViewProjection;

        Mesh m_cube;
        Mesh m_sphere;

        GLuint m_prog;

        Assimp::Importer m_meshImporter;

        Mesh loadMesh(const std::string& name);

    public:
        OpenGL(const std::shared_ptr<interface::IMath>& math);

        virtual void init()                                               override;
        virtual void setProjection(const interface::math::Matrix4x4& mat) override;
        virtual void setView(const interface::math::Matrix4x4& mat)       override;
        virtual void setModel(const interface::math::Matrix4x4& mat)      override;
        virtual void clear(float r, float g, float b, float a)            override;
        virtual void drawCube()                                           override;
        virtual void drawSphere()                                         override;
    };
}
