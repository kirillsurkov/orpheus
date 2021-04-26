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
        static const inline std::string vss_2d = "#version 330 core\n"
                   "uniform mat4 u_mvp;\n"
                   "layout(location = 0) in vec2 position;\n"
                   "layout(location = 1) in vec2 uv;\n"
                   "out vec2 v_uv;\n"
                   "void main() {\n"
                   "    v_uv = uv;\n"
                   "    gl_Position = u_mvp * vec4(position, -1.0, 1.0);\n"
                   "}";

        static const inline std::string vss_3d = "#version 330 core\n"
                   "uniform mat4 u_mvp;\n"
                   "layout(location = 0) in vec3 position;\n"
                   "layout(location = 1) in vec2 uv;\n"
                   "out vec2 v_uv;\n"
                   "void main() {\n"
                   "    v_uv = uv;\n"
                   "    gl_Position = u_mvp * vec4(position, 1.0);\n"
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

        Shader::Shader m_vertex2D;
        Shader::Shader m_vertex3D;
        Shader::Shader m_fragment;
        Shader::Program m_program2D;
        Shader::Program m_program3D;

        Math::Matrix4 m_projection;
        Math::Matrix4 m_view;
        Math::Matrix4 m_model;
        Math::Matrix4 m_glyphModel;

        std::size_t m_textureIndex;
        Math::Vector2 m_textureSize;
        float m_outline;
        Math::Color m_outlineColor;
        Math::Color m_textColor;

        void onCommand(const Orpheus::Material::Command::Prepare& command) {
            Shader::Program* program;

            const auto& vertices = command.getVertices();
            if (m_program2D.matchLayout(vertices)) {
                program = &m_program2D;
            } else if (m_program3D.matchLayout(vertices)) {
                program = &m_program3D;
            } else {
                throw std::runtime_error("Unsupported layout for material Text");
            }

            auto mvp = m_projection.mul(m_view).mul(m_model).mul(m_glyphModel);

            program->use();

            glActiveTexture(GL_TEXTURE0 + m_textureIndex);
            glUniform1i(program->getUniform("u_texture"), m_textureIndex);
            glUniform2f(program->getUniform("u_textureSize"), m_textureSize.getX(), m_textureSize.getY());
            glUniform1f(program->getUniform("u_outline"), m_outline);
            glUniform4f(program->getUniform("u_outlineColor"), m_outlineColor.getR(), m_outlineColor.getG(), m_outlineColor.getB(), m_outlineColor.getA());
            glUniform4f(program->getUniform("u_textColor"), m_textColor.getR(), m_textColor.getG(), m_textColor.getB(), m_textColor.getA());
            glUniformMatrix4fv(program->getUniform("u_mvp"), 1, GL_FALSE, mvp.getData());
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
            m_textureSize.set(command.getTextureWidth(), command.getTextureHeight());
        }

        void onCommand(const Orpheus::Material::Command::Texture& command) {
            m_textureIndex = command.getIndex();
        }

        void onCommand(const Orpheus::Material::Command::Color& command) {
            m_textColor = command.getColor();
        }

        void onCommand(const Orpheus::Material::Text::Command::GlyphAppearance& command) {
            m_outline = command.getOutline();
            m_outlineColor = command.getOutlineColor();
        }

    public:
        Text() :
            m_vertex2D(Shader::Type::VERTEX, vss_2d),
            m_vertex3D(Shader::Type::VERTEX, vss_3d),
            m_fragment(Shader::Type::FRAGMENT, fss)
        {
            m_program2D
                .attach(m_vertex2D)
                .attach(m_fragment)
                .addLayout(0, 2)
                .addLayout(1, 2)
                .link();

            m_program3D
                .attach(m_vertex3D)
                .attach(m_fragment)
                .addLayout(0, 3)
                .addLayout(1, 2)
                .link();

            postCommand(Orpheus::Material::Text::Command::GlyphAppearance(Orpheus::Material::Text::Appearance(), 0.0f));

            registerMaterialCommand<Orpheus::Material::Command::Prepare>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixProjection>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixView>(this);
            registerMaterialCommand<Orpheus::Material::Command::MatrixModel>(this);
            registerMaterialCommand<Orpheus::Material::Text::Command::GlyphModel>(this);
            registerMaterialCommand<Orpheus::Material::Command::Texture>(this);
            registerMaterialCommand<Orpheus::Material::Command::Color>(this);
            registerMaterialCommand<Orpheus::Material::Text::Command::GlyphAppearance>(this);
        }

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
