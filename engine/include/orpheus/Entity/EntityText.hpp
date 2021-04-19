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
#include "orpheus/Material/Text/Command/CommandGlyphAppearance.hpp"
#include "orpheus/Math/Vector.hpp"

namespace Orpheus::Entity {
    class Text : public Entity {
    private:
        Math::Vector2 m_coords;
        Math::Vector2 m_size;
        float m_textHeight;
        std::string m_text;
        std::string m_font;
        Math::Color m_color;
        Render::Command::GetTextSize m_sizeGetter;
        Render::Command::Material<Material::Text> m_material;
        Material::Text::Appearance m_appearance;

        Math::Matrix4 getTransform() const {
            return Math::Matrix4().translate(m_coords.getX(), m_coords.getY(), 0.0f);
        }

    public:
        Text(Vertex::BufferCache&, float x, float y, float height, const std::string& text, const std::string& font = "ubuntu-mono") :
            m_coords(x, y),
            m_textHeight(height),
            m_text(text),
            m_font(font),
            m_color(0.0f, 0.0f, 0.0f, 1.0f),
            m_sizeGetter(m_size, m_textHeight, m_text, m_font)
        {
        }

        virtual void draw(const Math::Matrix4& projection, const Math::Matrix4& view, Render::Render& render) const override {
            render.postCommand(m_material);
            render.postCommand(Material::Command::Color(m_color));
            render.postCommand(Material::Command::MatrixProjection(projection));
            render.postCommand(Material::Command::MatrixView(view));
            render.postCommand(Material::Command::MatrixModel(getTransform()));
            render.postCommand(Render::Command::Text(m_textHeight, m_text, m_font, m_appearance));
            render.postCommand(m_sizeGetter);
        }

        Math::Color& getColor() {
            return m_color;
        }

        Material::Text::Appearance& getAppearance() {
            return m_appearance;
        }

        const std::string& getText() const {
            return m_text;
        }

        float getX() const {
            return m_coords.getX();
        }

        float getY() const {
            return m_coords.getY();
        }

        float getWidth() const {
            return m_size.getX();
        }

        float getHeight() const {
            return m_size.getY();
        }

        void setX(float x) {
            m_coords.setX(x);
        }

        void setY(float y) {
            m_coords.setY(y);
        }

        void setText(const std::string& text) {
            m_text = text;
        }
    };
}
