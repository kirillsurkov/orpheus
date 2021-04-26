#pragma once

#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Material/Command/CommandPrepare.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Material/Command/CommandMatrixView.hpp"
#include "orpheus/Material/Command/CommandMatrixModel.hpp"

namespace Orpheus::Render::OpenGLImpl::Material {
    class FlatColor : public Material {
    private:
        static const inline std::string vss_2d = "#version 330 core\n"
                   "layout(location = 0) in vec2 position;\n"
                   "uniform mat4 u_mvp;\n"
                   "void main() {\n"
                   "    gl_Position = u_mvp * vec4(position, 0.0f, 1.0);\n"
                   "}";

        static const inline std::string vss_3d = "#version 330 core\n"
                   "layout(location = 0) in vec3 position;\n"
                   "uniform mat4 u_mvp;\n"
                   "void main() {\n"
                   "    gl_Position = u_mvp * vec4(position, 1.0);\n"
                   "}";

        static const inline std::string fss = "#version 330 core\n"
                   "uniform vec4 u_color;\n"
                   "out vec4 color;\n"
                   "void main() {\n"
                   "    color = vec4(u_color.r, u_color.g, u_color.b, u_color.a);\n"
                   "}";

        Shader::Shader m_vertex2D;
        Shader::Shader m_vertex3D;
        Shader::Shader m_fragment;
        Shader::Program m_program2D;
        Shader::Program m_program3D;

        Math::Color m_color;
        Math::Matrix4 m_projection;
        Math::Matrix4 m_view;
        Math::Matrix4 m_model;

        void onCommand(const Orpheus::Material::Command::Prepare& command) {
            Shader::Program* program;

            const auto& vertices = command.getVertices();
            if (m_program2D.matchLayout(vertices)) {
                program = &m_program2D;
            } else if (m_program3D.matchLayout(vertices)) {
                program = &m_program3D;
            } else {
                throw std::runtime_error("Unsupported layout for material FlatColor");
            }

            program->use();

            auto mvp = m_projection.mul(m_view).mul(m_model);

            glUniformMatrix4fv(program->getUniform("u_mvp"), 1, GL_FALSE, mvp.getData());
            glUniform4f(program->getUniform("u_color"), m_color.getR(), m_color.getG(), m_color.getB(), m_color.getA());
        }

        void onCommand(const Orpheus::Material::Command::Color& command) {
            m_color = command.getColor();
        }

        void onCommand(const Orpheus::Material::Command::MatrixProjection& command) {
            m_projection = command.getMatrix();
        }

        void onCommand(const Orpheus::Material::Command::MatrixView& command) {
            m_view = command.getMatrix();
        }

        void onCommand(const Orpheus::Material::Command::MatrixModel& command) {
            m_model = command.getMatrix();
        }

    public:
        FlatColor() :
            m_vertex2D(Shader::Type::VERTEX, vss_2d),
            m_vertex3D(Shader::Type::VERTEX, vss_3d),
            m_fragment(Shader::Type::FRAGMENT, fss)
        {
            m_program2D
                .attach(m_vertex2D)
                .attach(m_fragment)
                .addLayout(0, 2)
                .link();

            m_program3D
                .attach(m_vertex3D)
                .attach(m_fragment)
                .addLayout(0, 3)
                .link();

            registerMaterialCommand<Orpheus::Material::Command::Prepare>(this);
            registerMaterialCommand<Orpheus::Material::Command::Color>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixProjection>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixView>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixModel>(this);
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
