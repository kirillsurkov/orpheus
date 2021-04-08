#pragma once

#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Command/Material/CommandPrepare.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"
#include "orpheus/Command/Material/CommandMatrixModel.hpp"
#include "orpheus/Command/Material/CommandText.hpp"

namespace Orpheus::Render::OpenGLImpl::Material {
    class MaterialText : public Material {
    private:
        static const inline std::string vss = "#version 330 core\n"
                   "layout(location = 0) in vec3 position;\n"
                   "uniform mat4 u_model;\n"
                   "void main() {\n"
                   "    gl_Position = u_model * vec4(position, 1.0);\n"
                   "}";

        static const inline std::string fss = "#version 330 core\n"
                   "uniform vec4 u_color;\n"
                   "out vec4 color;\n"
                   "void main() {\n"
                   "    color = vec4(u_color.r, u_color.g, u_color.b, u_color.a);\n"
                   "}";

        int m_uModel;
        int m_uColor;

        void onCommand(const Command::Material::CommandPrepare&) {
        }

        void onCommand(const Command::Material::CommandColor& command) {
            glUniform4f(m_uColor, command.getR(), command.getG(), command.getB(), command.getA());
        }

        void onCommand(const Command::Material::CommandMatrixModel& command) {
            glUniformMatrix4fv(m_uModel, 1, GL_FALSE, &command.getMatrix()[0][0]);
        }

        void onCommand(const Command::Material::CommandText& command) {
        }

    public:
        MaterialText() : Material(vss, fss) {
            m_uColor = glGetUniformLocation(m_program, "u_color");
            m_uModel = glGetUniformLocation(m_program, "u_model");

            registerMaterialCommand<Command::Material::CommandPrepare>(this);
            registerMaterialCommand<Command::Material::CommandColor>(this);
            registerMaterialCommand<Command::Material::CommandMatrixModel>(this);
            registerMaterialCommand<Command::Material::CommandText>(this);
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
