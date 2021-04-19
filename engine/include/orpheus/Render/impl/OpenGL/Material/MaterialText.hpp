#pragma once

#include "orpheus/Render/impl/OpenGL/Material/Material.hpp"
#include "orpheus/Material/Command/CommandPrepare.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"
#include "orpheus/Material/Command/CommandTexture.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Material/Command/CommandMatrixView.hpp"
#include "orpheus/Material/Command/CommandMatrixModel.hpp"
#include "orpheus/Material/Text/Command/CommandGlyphModel.hpp"
#include "orpheus/Material/Text/Command/CommandGlyphAppearance.hpp"
#include "orpheus/Material/Text/TextAppearance.hpp"

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
                   "uniform sampler2D u_texture;\n"
                   "uniform vec2      u_textureSize;\n"
                   "uniform vec4      u_textColor;\n"
                   "uniform vec4      u_outlineColor;\n"
                   "uniform float     u_outline;\n"
                   "in vec2 v_uv;\n"
                   "out vec4 color;\n"
                   "float median(float r, float g, float b) {\n"
                   "    return max(min(r, g), min(max(r, g), b));\n"
                   "}\n"
                   "float antialias(float aa, float value) {\n"
                   "    return smoothstep(-0.75, 0.75, aa * (value - 0.5));\n"
                   "}\n"
                   "void main() {\n"
                   "    vec2 dPx = vec2(dFdx(v_uv.x), dFdy(v_uv.y)) * u_textureSize * textureSize(u_texture, 0);\n"
                   "    float aa = 1.4142135623730950488 * inversesqrt(dPx.x * dPx.x + dPx.y * dPx.y);\n"
                   "    vec4 mtsdf = texture(u_texture, v_uv);\n"
                   "    float alpha = antialias(aa, median(mtsdf.r, mtsdf.g, mtsdf.b));\n"
                   "    float outline = antialias(aa, mtsdf.a + u_outline);\n"
                   "    vec4 glyphColor = vec4(u_textColor.xyz, 1.0) * u_textColor.a * alpha;\n"
                   "    vec4 outlineColor = vec4(u_outlineColor.xyz, 1.0) * u_outlineColor.a * outline * (1.0 - glyphColor.a);\n"
                   "    color = glyphColor + outlineColor;\n"
                   "}";

        int m_uMVP;
        int m_uTexture;
        int m_uTextureSize;
        int m_uTextColor;
        int m_uOutlineColor;
        int m_uOutline;

        Math::Matrix4 m_projection;
        Math::Matrix4 m_view;
        Math::Matrix4 m_model;
        Math::Matrix4 m_glyphModel;

        void onCommand(const Orpheus::Material::Command::Prepare&) {
            auto mvp = m_projection.mul(m_view).mul(m_model).mul(m_glyphModel);
            glUniformMatrix4fv(m_uMVP, 1, GL_FALSE, mvp.getData());
        }

        void onCommand(const Orpheus::Material::Command::Color& command) {
            const auto& color = command.getColor();
            glUniform4f(m_uTextColor, color.getR(), color.getG(), color.getB(), color.getA());
        }

        void onCommand(const Orpheus::Material::Command::Texture& command) {
            glActiveTexture(GL_TEXTURE0 + command.getIndex());
            glUniform1i(m_uTexture, command.getIndex());
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
            glUniform2f(m_uTextureSize, command.getTextureWidth(), command.getTextureHeight());
        }

        void onCommand(const Orpheus::Material::Text::Command::GlyphAppearance& command) {
            const auto& outlineColor = command.getOutlineColor();
            glUniform4f(m_uOutlineColor, outlineColor.getR(), outlineColor.getG(), outlineColor.getB(), outlineColor.getA());
            glUniform1f(m_uOutline, command.getOutline());
        }

    public:
        Text() : Material(vss, fss) {
            m_uMVP = glGetUniformLocation(m_program, "u_mvp");
            m_uTexture = glGetUniformLocation(m_program, "u_texture");
            m_uTextureSize = glGetUniformLocation(m_program, "u_textureSize");
            m_uTextColor = glGetUniformLocation(m_program, "u_textColor");
            m_uOutlineColor = glGetUniformLocation(m_program, "u_outlineColor");
            m_uOutline = glGetUniformLocation(m_program, "u_outline");

            postCommand(Orpheus::Material::Text::Command::GlyphAppearance(Orpheus::Material::Text::Appearance(), 0.0f));

            registerMaterialCommand<Orpheus::Material::Command::Prepare>(this);
            registerMaterialCommand<Orpheus::Material::Command::Color>(this);
            registerMaterialCommand<Orpheus::Material::Command::Texture>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixProjection>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixView>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixModel>(this);
            registerMaterialCommand<Orpheus::Material::Text::Command::GlyphModel>(this);
            registerMaterialCommand<Orpheus::Material::Text::Command::GlyphAppearance>(this);
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
