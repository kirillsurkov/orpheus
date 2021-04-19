#pragma once

#include "orpheus/Entity/EntityText.hpp"
#include "orpheus/Entity/EntityRect.hpp"

namespace Orpheus::Entity::UI {
    class Button : public Entity {
    private:
        std::function<void()> m_onClick;
        Rect m_rect;
        Rect m_rectDisabled;
        Text m_text;
        float m_timer = 0.0f;
        float m_mouseX = 0.0f;
        float m_mouseY = 0.0f;
        bool m_enabled = true;
        bool m_clicked = false;
        bool m_lastClicked = false;

    public:
        Button(Vertex::BufferCache& bufferCache, float x, float y, const std::string& text, const std::function<void()>& onClick, float height = 32.0f) :
            m_onClick(onClick),
            m_rect(bufferCache, x, y, 0.0f, 0.0f),
            m_rectDisabled(bufferCache, x, y, 0.0f, 0.0f),
            m_text(bufferCache, x, y, height, text)
        {
            m_rectDisabled.getColor().set(0.5f, 0.5f, 0.5f, 0.75f);
            m_text.getColor().set(0.0f, 0.0f, 0.0f, 1.0f);
        }

        virtual void draw(const Math::Matrix4& projection, const Math::Matrix4& view, Render::Render& render) const override {
            m_rect.draw(projection, view, render);
            m_text.draw(projection, view, render);
            if (!m_enabled) {
                m_rectDisabled.draw(projection, view, render);
            }
        }

        virtual void update(float delta) override {
            m_timer = std::fmod(m_timer + delta, 2.0f);

            bool insideX = m_mouseX >= m_rect.getX() && m_mouseX <= m_rect.getX() + m_rect.getWidth();
            bool insideY = m_mouseY >= m_rect.getY() && m_mouseY <= m_rect.getY() + m_rect.getHeight();
            if (insideX && insideY) {
                float value = m_clicked ? 0.5f : 0.75f;

                m_rect.getColor().set(Math::Color::fromHSV(m_timer * 180.0f, 0.5f, value));

                if (m_enabled && !m_clicked && m_lastClicked) {
                    m_onClick();
                }
            } else {
                m_rect.getColor().set(0.75f, 0.75f, 0.75f, 1.0f);
            }

            m_rect.setWidth(m_text.getWidth());
            m_rect.setHeight(m_text.getHeight());
            m_rectDisabled.setWidth(m_text.getWidth());
            m_rectDisabled.setHeight(m_text.getHeight());

            m_lastClicked = m_clicked;
        }

        bool isEnabled() const {
            return m_enabled;
        }

        float getX() const {
            return m_rect.getX();
        }

        float getY() const {
            return m_rect.getY();
        }

        float getWidth() const {
            return m_rect.getWidth();
        }

        float getHeight() const {
            return m_rect.getHeight();
        }

        void setEnabled(bool enabled) {
            m_enabled = enabled;
        }

        void setMousePos(float x, float y) {
            m_mouseX = x;
            m_mouseY = y;
        }

        void setMouseClicked(bool clicked) {
            m_clicked = clicked;
        }

        void setX(float x) {
            m_rect.setX(x);
            m_rectDisabled.setX(x);
            m_text.setX(x);
        }

        void setY(float y) {
            m_rect.setY(y);
            m_rectDisabled.setY(y);
            m_text.setY(y);
        }
    };
}
