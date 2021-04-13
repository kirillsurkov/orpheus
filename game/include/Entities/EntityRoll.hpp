#pragma once

#include "Entities/EntityReel.hpp"

class EntityRoll : public Orpheus::Entity::Entity {
private:
    std::vector<EntityReel> m_reels;
    std::size_t m_spinningReel;
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 0.0f;
    float m_height = 0.0f;
    float m_margin = 0.0f;

public:
    EntityRoll(Orpheus::Vertex::BufferCache& bufferCache, std::size_t rows, std::size_t cols) {
        for (std::size_t i = 0; i < cols; i++) {
            m_reels.emplace_back(bufferCache, rows);
        }
        m_spinningReel = m_reels.size();
    }

    virtual void update(float delta) override {
        for (auto& reel : m_reels) {
            reel.update(delta);
        }
    }

    virtual void draw(const glm::mat4& projection, const glm::mat4& view, Orpheus::Render::Render& render) override {
        float step = (m_width - m_margin) / m_reels.size();
        float width = step - m_margin;
        for (std::size_t i = 0; i < m_reels.size(); i++) {
            auto& reel = m_reels[i];
            reel.setX(m_x + step * i);
            reel.setY(m_y);
            reel.setWidth(width);
            reel.setHeight(m_height);
            reel.setMargin(m_margin);
            reel.draw(projection, view, render);
        }
    }

    std::string getRow(std::size_t row) const {
        std::string res;
        for (const auto& reel : m_reels) {
            res += reel.getRow(row);
        }
        return res;
    }

    void start() {
        m_spinningReel = 0;
        float baseSpeed = 6.0f;
        for (auto& reel : m_reels) {
            reel.start(baseSpeed + 3.0f * rand() / static_cast<float>(RAND_MAX));
        }
    }

    bool split() {
        if (isSpinning()) {
            m_reels[m_spinningReel++].stop();
        }
        return isSpinning();
    }

    void setX(float x) {
        m_x = x;
    }

    void setY(float y) {
        m_y = y;
    }

    void setWidth(float width) {
        m_width = width;
    }

    void setHeight(float height) {
        m_height = height;
    }

    void setMargin(float margin) {
        m_margin = margin;
    }

    bool isSpinning() const {
        return m_spinningReel < m_reels.size();
    }
};
