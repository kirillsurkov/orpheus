#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Command/Render/CommandMaterial.hpp"
#include "orpheus/Command/Render/CommandVertices.hpp"
#include "orpheus/Command/Material/CommandColor.hpp"
#include "orpheus/Command/Material/CommandMatrixProjection.hpp"
#include "orpheus/Command/Material/CommandMatrixView.hpp"
#include "orpheus/Command/Material/CommandMatrixModel.hpp"
#include "orpheus/Material/MaterialFlatColor.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace Orpheus::Entity {
    class EntityRect : public Entity {
    private:
        float m_x;
        float m_y;
        float m_w;
        float m_h;
        Command::Render::CommandMaterial<Material::MaterialFlatColor> m_material;
        Command::Render::CommandVertices m_vertices;
        Command::Material::CommandColor m_color;

        glm::mat4x4 getTransform() const {
            glm::mat4x4 res(1.0f);
            res = glm::translate(res, glm::vec3(m_x, m_y, 0.0f));
            res = glm::scale(res, glm::vec3(m_w, m_h, 1.0f));
            return res;
        }

    public:
        EntityRect(float x, float y, float w, float h) :
            m_x(x), m_y(y),
            m_w(w), m_h(h),
            m_material(),
            m_vertices(),
            m_color(0.0f, 1.0f, 1.0f, 1.0f)
        {
            auto& positions = m_vertices.addAttrib(0, 2);
            positions->addPoint(-0.5f, -0.5f);
            positions->addPoint( 0.5f, -0.5f);
            positions->addPoint( 0.5f,  0.5f);
            positions->addPoint( 0.5f,  0.5f);
            positions->addPoint(-0.5f, -0.5f);
            positions->addPoint(-0.5f,  0.5f);
        }

        void setX(float x) {
            m_x = x;
        }

        void setY(float y) {
            m_y = y;
        }

        virtual void draw(const glm::mat4x4& projection, const glm::mat4x4& view, Render::Render& render) override {
            render.postCommand(m_material);
            render.postCommand(Command::Material::CommandMatrixProjection(projection));
            render.postCommand(Command::Material::CommandMatrixView(view));
            render.postCommand(Command::Material::CommandMatrixModel(getTransform()));
            render.postCommand(m_color);
            render.postCommand(m_vertices);
        }
    };
}
