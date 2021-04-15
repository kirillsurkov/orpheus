#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Render/Command/CommandText.hpp"
#include "orpheus/Render/Command/CommandMaterial.hpp"
#include "orpheus/Render/Command/CommandGetTextSize.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Material/Command/CommandMatrixView.hpp"
#include "orpheus/Material/Command/CommandMatrixModel.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"

namespace Orpheus::Entity {
    class Text : public Entity {
    private:
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_textHeight;
        std::string m_text;
        std::string m_font;
        Material::Command::Color m_color;
        Render::Command::GetTextSize m_sizeGetter;
        Render::Command::Material<Material::Text> m_material;

        glm::mat4x4 getTransform() const {
            return glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, 0.0f));
        }

    public:
        Text(Vertex::BufferCache&, float x, float y, float height, const std::string& text, const std::string& font = "ubuntu-mono") :
            m_x(x),
            m_y(y),
            m_textHeight(height),
            m_text(text),
            m_font(font),
            m_color(0.0f, 0.0f, 0.0f, 1.0f),
            m_sizeGetter(m_width, m_height, m_textHeight, m_text, m_font)
        {
        }

        virtual void draw(const glm::mat4x4& projection, const glm::mat4x4& view, Render::Render& render) const override {
            render.postCommand(m_material);
            render.postCommand(m_color);
            render.postCommand(Material::Command::MatrixProjection(projection));
            render.postCommand(Material::Command::MatrixView(view));
            render.postCommand(Material::Command::MatrixModel(getTransform()));
            render.postCommand(Render::Command::Text(m_textHeight, m_text, m_font));
            render.postCommand(m_sizeGetter);
        }

        Material::Command::Color& getColor() {
            return m_color;
        }

        const std::string& getText() const {
            return m_text;
        }

        float getWidth() const {
            return m_width;
        }

        float getHeight() const {
            return m_height;
        }

        void setX(float x) {
            m_x = x;
        }

        void setY(float y) {
            m_y = y;
        }

        void setText(const std::string& text) {
            m_text = text;
        }
    };
}
