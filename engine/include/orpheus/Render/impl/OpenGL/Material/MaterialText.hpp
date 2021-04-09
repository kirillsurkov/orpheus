#pragma once

#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Command/Material/CommandPrepare.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"
#include "orpheus/Command/Material/CommandTexture.hpp"
#include "orpheus/Command/Material/CommandMatrixProjection.hpp"
#include "orpheus/Command/Material/CommandMatrixView.hpp"
#include "orpheus/Command/Material/CommandMatrixModel.hpp"
#include "orpheus/Command/Material/CommandTextRect.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Orpheus::Render::OpenGLImpl::Material {
    class MaterialText : public Material {
    private:
        static const inline std::string vss = "#version 330 core\n"
                   "layout(location = 0) in vec2 position;\n"
                   "layout(location = 1) in vec2 uv;\n"
                   "uniform mat4 u_mvp;\n"
                   "out vec2 v_uv;\n"
                   "void main() {\n"
                   "    v_uv = uv;\n"
                   "    gl_Position = u_mvp * vec4(position, -1.0, 1.0);\n"
                   "}";

        static const inline std::string fss = "#version 330 core\n"
                   "uniform vec4 u_color;\n"
                   "uniform sampler2D u_texture;\n"
                   "in vec2 v_uv;\n"
                   "out vec4 color;\n"
                   "float median(float r, float g, float b) {\n"
                   "    return max(min(r, g), min(max(r, g), b));\n"
                   "}\n"
                   "void main() {\n"
                   "    vec4 msdf = texture(u_texture, v_uv);\n"
                   "    float dist = median(msdf.r, msdf.g, msdf.b);\n"
                   "    float width = fwidth(dist);\n"
                   "    float alpha = smoothstep(0.5 - width, 0.5 + width, dist);\n"
                   "    color = vec4(1.0, 0.0, 0.0, alpha);\n"
                   "}";

        int m_uMVP;
        int m_uColor;
        int m_uTexture;

        glm::mat4x4 m_projection;
        glm::mat4x4 m_view;
        glm::mat4x4 m_model;

        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_advance;

        void onCommand(const Command::Material::CommandPrepare&) {
            auto mvp = m_projection * m_view;
            mvp = glm::translate(mvp, glm::vec3(m_advance + m_x, m_y, 0.0f));
            mvp = glm::scale(mvp, glm::vec3(m_width, m_height, 1.0f));
            glUniformMatrix4fv(m_uMVP, 1, GL_FALSE, &mvp[0][0]);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        void onCommand(const Command::Material::CommandColor& command) {
            glUniform4f(m_uColor, command.getR(), command.getG(), command.getB(), command.getA());
        }

        void onCommand(const Command::Material::CommandTexture& command) {
            const auto& id = command.getTextureId();
            glBindTexture(GL_TEXTURE_2D, id);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(m_uTexture, 0);
        }

        void onCommand(const Command::Material::CommandMatrixProjection& command) {
            m_projection = command.getMatrix();
            m_model = glm::mat4(1.0f);
        }

        void onCommand(const Command::Material::CommandMatrixView& command) {
            m_view = command.getMatrix();
        }

        void onCommand(const Command::Material::CommandMatrixModel& command) {
            m_model = m_model * command.getMatrix();
        }

        void onCommand(const Command::Material::Text::CommandRect& command) {
            m_x = command.getX();
            m_y = command.getY();
            m_width = command.getWidth();
            m_height = command.getHeight();
            m_advance = command.getAdvance();
        }

    public:
        MaterialText() : Material(vss, fss) {
            m_uMVP = glGetUniformLocation(m_program, "u_mvp");
            m_uColor = glGetUniformLocation(m_program, "u_color");
            m_uTexture = glGetUniformLocation(m_program, "u_texture");

            registerMaterialCommand<Command::Material::CommandPrepare>(this);
            registerMaterialCommand<Command::Material::CommandColor>(this);
            registerMaterialCommand<Command::Material::CommandTexture>(this);
            registerMaterialCommand<Command::Material::CommandMatrixProjection>(this);
            registerMaterialCommand<Command::Material::CommandMatrixView>(this);
            registerMaterialCommand<Command::Material::CommandMatrixModel>(this);
            registerMaterialCommand<Command::Material::Text::CommandRect>(this);
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
