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
        static const inline std::string vss = "#version 330 core\n"
                   "layout(location = 0) in vec2 position;\n"
                   "uniform mat4 u_mvp;\n"
                   "void main() {\n"
                   "    gl_Position = u_mvp * vec4(position, 0.0f, 1.0);\n"
                   "}";

        static const inline std::string fss = "#version 330 core\n"
                   "uniform vec4 u_color;\n"
                   "out vec4 color;\n"
                   "void main() {\n"
                   "    color = vec4(u_color.r, u_color.g, u_color.b, u_color.a);\n"
                   "}";

        int m_uColor;
        int m_uMVP;

        glm::mat4x4 m_projection;
        glm::mat4x4 m_view;
        glm::mat4x4 m_model;

        void onCommand(const Orpheus::Material::Command::Prepare&) {
            auto mvp = m_projection * m_view * m_model;
            glUniformMatrix4fv(m_uMVP, 1, GL_FALSE, &mvp[0][0]);
        }

        void onCommand(const Orpheus::Material::Command::Color& command) {
            glUniform4f(m_uColor, command.getR(), command.getG(), command.getB(), command.getA());
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
        FlatColor() : Material(vss, fss) {
            m_uColor = glGetUniformLocation(m_program, "u_color");
            m_uMVP = glGetUniformLocation(m_program, "u_mvp");

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
