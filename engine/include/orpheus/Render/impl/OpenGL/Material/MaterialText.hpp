#pragma once

#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Material/Command/CommandPrepare.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"
#include "orpheus/Material/Command/CommandTexture.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Material/Command/CommandMatrixView.hpp"
#include "orpheus/Material/Command/CommandMatrixModel.hpp"
#include "orpheus/Material/Text/Command/CommandGlyphModel.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Orpheus::Render::OpenGLImpl::Material {
    class Text : public Material {
    private:
        static const inline std::string vss = "#version 330 core\n"
                   "uniform mat4 u_mvp;\n"
                   "layout(location = 0) in vec2 position;\n"
                   "layout(location = 1) in vec2 uv;\n"
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
                   "    color = vec4(u_color.rgb, alpha);\n"
                   "}";

        int m_uMVP;
        int m_uColor;
        int m_uTexture;

        glm::mat4x4 m_projection;
        glm::mat4x4 m_view;
        glm::mat4x4 m_model;
        glm::mat4x4 m_glyphModel;

        void onCommand(const Orpheus::Material::Command::Prepare&) {
            auto mvp = m_projection * m_view * m_model * m_glyphModel;
            glUniformMatrix4fv(m_uMVP, 1, GL_FALSE, &mvp[0][0]);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        void onCommand(const Orpheus::Material::Command::Color& command) {
            glUniform4f(m_uColor, command.getR(), command.getG(), command.getB(), command.getA());
        }

        void onCommand(const Orpheus::Material::Command::Texture& command) {
            const auto& id = command.getTextureId();
            glBindTexture(GL_TEXTURE_2D, id);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(m_uTexture, 0);
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

        void onCommand(const Orpheus::Material::Text::Command::GlyphModel& command) {
            m_glyphModel = command.getMatrix();
        }

    public:
        Text() : Material(vss, fss) {
            m_uMVP = glGetUniformLocation(m_program, "u_mvp");
            m_uColor = glGetUniformLocation(m_program, "u_color");
            m_uTexture = glGetUniformLocation(m_program, "u_texture");

            registerMaterialCommand<Orpheus::Material::Command::Prepare>(this);
            registerMaterialCommand<Orpheus::Material::Command::Color>(this);
            registerMaterialCommand<Orpheus::Material::Command::Texture>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixProjection>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixView>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixModel>(this);
            registerMaterialCommand<Orpheus::Material::Text::Command::GlyphModel>(this);
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
