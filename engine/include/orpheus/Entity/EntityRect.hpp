#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Render/Command/CommandMaterial.hpp"
#include "orpheus/Render/Command/CommandVertices.hpp"
#include "orpheus/Material/Command/CommandColor.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Material/Command/CommandMatrixView.hpp"
#include "orpheus/Material/Command/CommandMatrixModel.hpp"
#include "orpheus/Material/FlatColor/MaterialFlatColor.hpp"
#include "orpheus/Math/Vector.hpp"

namespace Orpheus::Entity {
    class Rect : public Entity {
    private:
        Math::Vector2 m_coords;
        Math::Vector2 m_size;
        Vertex::Vertices m_mesh;
        Render::Command::Material<Material::FlatColor> m_material;
        Math::Color m_color;

        Math::Matrix4 getTransform() const {
            return Math::Matrix4().translate(m_coords.getX(), m_coords.getY(), 0.0f).scale(m_size.getX(), m_size.getY(), 1.0f);
        }

    public:
        Rect(const Vertex::BufferCache& bufferCache, float x, float y, float w, float h) :
            m_coords(x, y),
            m_size(w, h),
            m_material(),
            m_color(0.0f, 1.0f, 1.0f, 1.0f)
        {
            m_mesh.addAttrib(0, bufferCache.get("rect_left_bottom"));
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

        void setWidth(float width) {
            m_size.setX(width);
        }

        void setHeight(float height) {
            m_size.setY(height);
        }

        Math::Color& getColor() {
            return m_color;
        }

        virtual void draw(const Math::Matrix4& projection, const Math::Matrix4& view, Render::Render& render) const override {
            render.postCommand(m_material);
            render.postCommand(Material::Command::MatrixProjection(projection));
            render.postCommand(Material::Command::MatrixView(view));
            render.postCommand(Material::Command::MatrixModel(getTransform()));
            render.postCommand(Material::Command::Color(m_color));
            render.postCommand(Render::Command::Vertices(m_mesh));
        }
    };
}
