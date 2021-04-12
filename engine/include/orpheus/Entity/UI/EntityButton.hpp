#pragma once

#include "orpheus/Entity/EntityText.hpp"
#include "orpheus/Entity/EntityRect.hpp"

#include <glm/gtx/color_space.hpp>

namespace Orpheus::Entity::UI {
    class Button : public Entity {
    private:
        Rect m_rect;
        Text m_text;
        float m_timer = 0.0f;
        float m_mouseX = 0.0f;
        float m_mouseY = 0.0f;
        bool m_clicked = false;

    public:
        Button(Vertex::BufferCache& bufferCache, float x, float y, const std::string& text) :
            m_rect(bufferCache, x, y, 0.2f, 32.0f / 400.0f),
            m_text(bufferCache, x, y, 32.0f, text)
        {
            m_text.getColor().set(0.0f, 0.0f, 0.0f, 1.0f);
        }

        virtual void draw(const glm::mat4x4& projection, const glm::mat4x4& view, Render::Render& render) override {
            m_rect.setWidth(m_text.getWidth(render));
            m_rect.draw(projection, view, render);
            m_text.draw(projection, view, render);
        }

        virtual void update(float delta) override {
            m_timer = std::fmod(m_timer + delta, 1.0f);

            bool insideX = m_mouseX >= m_rect.getX() && m_mouseX <= m_rect.getX() + m_rect.getWidth();
            bool insideY = m_mouseY >= m_rect.getY() && m_mouseY <= m_rect.getY() + m_rect.getHeight();
            if (insideX && insideY) {
                if (m_clicked) {
                    auto color = glm::rgbColor(glm::vec3(m_timer * 360.0f, 0.5f, 0.5f));
                    m_rect.getColor().set(color.x, color.y, color.z, 1.0f);
                } else {
                    auto color = glm::rgbColor(glm::vec3(m_timer * 360.0f, 0.5f, 1.0f));
                    m_rect.getColor().set(color.x, color.y, color.z, 1.0f);
                }
            } else {
                m_rect.getColor().set(0.75f, 0.75f, 0.75f, 1.0f);
            }
        }

        void setMousePos(float x, float y) {
            m_mouseX = x;
            m_mouseY = y;
        }

        void setClicked(bool clicked) {
            m_clicked = clicked;
        }
    };
}
