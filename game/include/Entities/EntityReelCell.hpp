#pragma once

#include <orpheus/Entity/EntityText.hpp>
#include <orpheus/Entity/EntityRect.hpp>

class EntityReelCell : public Orpheus::Entity::Entity {
private:
    Orpheus::Entity::Rect m_rect;
    Orpheus::Entity::Text m_text;
    static constexpr float m_textHeight = 32.0f;

public:
    EntityReelCell(Orpheus::Vertex::BufferCache& bufferCache, const std::string& text) :
        m_rect(bufferCache, 0.0f, 0.0f, 0.0f, 0.0f),
        m_text(bufferCache, 0.0f, 0.0f, m_textHeight, text)
    {
    }

    virtual void draw(const glm::mat4& projection, const glm::mat4& view, Orpheus::Render::Render& render) override {
        m_text.setX(m_rect.getX() + 0.5f * (m_rect.getWidth() - m_text.getWidth(render)));
        m_text.setY(m_rect.getY() + 0.5f * (m_rect.getHeight() - m_text.getHeight()));
        m_rect.draw(projection, view, render);
        m_text.draw(projection, view, render);
    }

    float getY() const {
        return m_rect.getY();
    }

    float getHeight() const {
        return m_rect.getHeight();
    }

    const std::string& getText() const {
        return m_text.getText();
    }

    Orpheus::Material::Command::Color& getColor() {
        return m_rect.getColor();
    }

    void setX(float x) {
        m_rect.setX(x);
    }

    void setY(float y) {
        m_rect.setY(y);
    }

    void setWidth(float width) {
        m_rect.setWidth(width);
    }

    void setHeight(float height) {
        m_rect.setHeight(height);
    }
};
