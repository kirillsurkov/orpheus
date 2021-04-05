#pragma once

#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Command/Material/CommandPrepare.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"
#include "orpheus/Command/Material/CommandMatrixProjection.hpp"
#include "orpheus/Command/Material/CommandMatrixView.hpp"
#include "orpheus/Command/Material/CommandMatrixModel.hpp"

namespace Orpheus::Render::OpenGLImpl::Material {
    class MaterialFlatColor : public Material {
    private:
        static const inline std::string vss = "#version 330 core\n"
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

        int m_uColor;
        int m_uMVP;

        glm::mat4x4 m_projection;
        glm::mat4x4 m_view;
        glm::mat4x4 m_model;

        void onCommand(const Command::Material::CommandPrepare&) {
            auto mvp = m_projection * m_view * m_model;
            glUniformMatrix4fv(m_uMVP, 1, GL_FALSE, &mvp[0][0]);
        }

        void onCommand(const Command::Material::CommandColor& command) {
            glUniform4f(m_uColor, command.getR(), command.getG(), command.getB(), command.getA());
        }

        void onCommand(const Command::Material::CommandMatrixProjection& command) {
            m_projection = command.getMatrix();
        }

        void onCommand(const Command::Material::CommandMatrixView& command) {
            m_view = command.getMatrix();
        }

        void onCommand(const Command::Material::CommandMatrixModel& command) {
            m_model = command.getMatrix();
        }

    public:
        MaterialFlatColor() : Material(vss, fss) {
            m_uColor = glGetUniformLocation(m_program, "u_color");
            m_uMVP = glGetUniformLocation(m_program, "u_mvp");

            registerMaterialCommand<Command::Material::CommandPrepare>(this);
            registerMaterialCommand<Command::Material::CommandColor>(this);
            registerMaterialCommand<Command::Material::CommandMatrixProjection>(this);
            registerMaterialCommand<Command::Material::CommandMatrixView>(this);
            registerMaterialCommand<Command::Material::CommandMatrixModel>(this);
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
