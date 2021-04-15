#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Render/Command/CommandText.hpp"
#include "orpheus/Render/Command/CommandMaterial.hpp"
#include "orpheus/Render/Command/CommandGetTextWidth.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Material/Command/CommandMatrixView.hpp"
#include "orpheus/Material/Command/CommandMatrixModel.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"

namespace Orpheus::Entity {
    class Text : public Entity {
    private:
        static constexpr float HALF_SCREEN_W = 400.0f; // TODO: change it to something more reasonable

        float m_x;
        float m_y;
        float m_width;
        float m_height;
        std::string m_text;
        std::string m_font;
        Material::Command::Color m_color;
        Render::Command::GetTextWidth m_widthGetter;
        Render::Command::Material<Material::Text> m_material;

        glm::mat4x4 getTransform() const {
            glm::mat4x4 res(1.0f);
            res = glm::translate(res, glm::vec3(m_x, m_y, 0.0f));
            res = glm::scale(res, glm::vec3(m_height / HALF_SCREEN_W, m_height / HALF_SCREEN_W, 1.0f));
            return res;
        }

    public:
        Text(Vertex::BufferCache&, float x, float y, float height, const std::string& text, const std::string& font = "ubuntu-mono") :
            m_x(x),
            m_y(y),
            m_height(height),
            m_text(text),
            m_font(font),
            m_color(0.0f, 0.0f, 0.0f, 1.0f),
            m_widthGetter(m_width, m_height, m_text, m_font)
        {
        }

        virtual void draw(const glm::mat4x4& projection, const glm::mat4x4& view, Render::Render& render) const override {
            render.postCommand(m_material);
            render.postCommand(m_color);
            render.postCommand(Material::Command::MatrixProjection(projection));
            render.postCommand(Material::Command::MatrixView(view));
            render.postCommand(Material::Command::MatrixModel(getTransform()));
            render.postCommand(Render::Command::Text(m_x, m_y, m_height, m_text, m_font));
            render.postCommand(m_widthGetter);
        }

        Material::Command::Color& getColor() {
            return m_color;
        }

        const std::string& getText() const {
            return m_text;
        }

        float getWidth() const {
            return m_width * m_height / HALF_SCREEN_W;
        }

        float getHeight() const {
            return m_height / HALF_SCREEN_W;
        }

        float getTextHeight() const {
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
