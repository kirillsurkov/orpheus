#pragma once

#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Command/Render/CommandColor.hpp"

namespace Orpheus::Render::OpenGLImpl::Material {
    class MaterialFlatColor : public Material {
    private:
        static const inline std::string vss = "#version 330 core\n"
                   "layout(location = 0) in vec2 position;\n"
                   "void main() {\n"
                   "    gl_Position = vec4(position, 0.0, 1.0);\n"
                   "}";

        static const inline std::string fss = "#version 330 core\n"
                   "uniform vec4 u_color;\n"
                   "out vec4 color;\n"
                   "void main() {\n"
                   "    color = vec4(u_color.r, u_color.g, u_color.b, u_color.a);\n"
                   "}";

        void onCommand(const Command::Render::CommandColor& command) {
            glUniform4f(glGetUniformLocation(m_program, "u_color"), command.getR(), command.getG(), command.getB(), command.getA());
        }

    public:
        MaterialFlatColor() : Material(vss, fss) {
            registerMaterialCommand<Command::Render::CommandColor>(this);
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
