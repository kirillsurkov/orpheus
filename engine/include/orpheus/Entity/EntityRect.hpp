#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Render/Command/CommandMaterial.hpp"
#include "orpheus/Render/Command/CommandVertices.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Material/Command/CommandMatrixView.hpp"
#include "orpheus/Material/Command/CommandMatrixModel.hpp"
#include "orpheus/Material/FlatColor/MaterialFlatColor.hpp"

namespace Orpheus::Entity {
    class Rect : public Entity {
    private:
        float m_x;
        float m_y;
        float m_w;
        float m_h;
        Vertex::Vertices m_mesh;
        Render::Command::Material<Material::FlatColor> m_material;
        Material::Command::Color m_color;

        glm::mat4x4 getTransform() const {
            glm::mat4x4 res(1.0f);
            res = glm::translate(res, glm::vec3(m_x, m_y, 0.0f));
            res = glm::scale(res, glm::vec3(m_w, m_h, 1.0f));
            return res;
        }

    public:
        Rect(const Vertex::BufferCache& bufferCache, float x, float y, float w, float h) :
            m_x(x), m_y(y),
            m_w(w), m_h(h),
            m_material(),
            m_color(0.0f, 1.0f, 1.0f, 1.0f)
        {
            m_mesh.addAttrib(0, bufferCache.get("rect_left_bottom"));
        }

        float getX() const {
            return m_x;
        }

        float getY() const {
            return m_y;
        }

        float getWidth() const {
            return m_w;
        }

        float getHeight() const {
            return m_h;
        }

        void setX(float x) {
            m_x = x;
        }

        void setY(float y) {
            m_y = y;
        }

        void setWidth(float width) {
            m_w = width;
        }

        void setHeight(float height) {
            m_h = height;
        }

        Material::Command::Color& getColor() {
            return m_color;
        }

        virtual void draw(const glm::mat4x4& projection, const glm::mat4x4& view, Render::Render& render) const override {
            render.postCommand(m_material);
            render.postCommand(Material::Command::MatrixProjection(projection));
            render.postCommand(Material::Command::MatrixView(view));
            render.postCommand(Material::Command::MatrixModel(getTransform()));
            render.postCommand(m_color);
            render.postCommand(Render::Command::Vertices(m_mesh));
        }
    };
}
