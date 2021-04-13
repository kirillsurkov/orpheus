#pragma once

#include "Entities/EntityReelCell.hpp"

#include <deque>
#include <glm/gtx/color_space.hpp>

class EntityReel : public Orpheus::Entity::Entity {
private:
    std::deque<EntityReelCell> m_cells;
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 0.0f;
    float m_height = 0.0f;
    float m_margin = 0.0f;
    float m_offset = 0.0f;
    float m_speed = 0.0f;
    bool m_needStop = false;
    bool m_stopped = true;

public:
    EntityReel(Orpheus::Vertex::BufferCache& bufferCache, std::size_t rows) {
        for (std::size_t i = 0; i < rows; i++) {
            m_cells.emplace_back(bufferCache, std::to_string(i + 1));
            auto color = glm::rgbColor(glm::vec3(360.0f * i / rows, 0.5f, 1.0f));
            m_cells.back().getColor().set(color.r, color.g, color.b, 1.0f);
        }
    }

    virtual void update(float delta) override {
        if (m_needStop && m_offset > 0) {
            m_offset += (0 - m_offset) * (delta / 0.1f);
            m_stopped = true;
        } else
            m_offset -= delta * m_speed;
    }

    virtual void draw(const glm::mat4& projection, const glm::mat4& view, Orpheus::Render::Render& render) override {
        float step = (m_height - m_margin) / (m_cells.size() - 1);
        float height = step - m_margin;
        float y = (m_offset - 0.5f) * (height + m_margin);
        for (auto& cell : m_cells) {
            cell.setX(m_x);
            cell.setY(m_y + y);
            cell.setWidth(m_width);
            cell.setHeight(height);
            cell.draw(projection, view, render);
            y += step;
        }

        auto& firstCell = m_cells.front();
        if (firstCell.getY() + firstCell.getHeight() <= m_y) {
            m_offset += 1.0f;
            std::rotate(m_cells.begin(), m_cells.begin() + 1, m_cells.end());
        }
    }

    bool isSpinning() const {
        return !m_stopped;
    }

    std::string getRow(std::size_t index) const {
        return m_cells[index].getText();
    }

    void start(float speed) {
        m_speed = speed;
        m_needStop = false;
        m_stopped = false;
    }

    void stop() {
        m_needStop = true;
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
};
