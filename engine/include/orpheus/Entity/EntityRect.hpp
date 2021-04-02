#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Command/Render/CommandMaterial.hpp"
#include "orpheus/Command/Render/CommandVertices.hpp"
#include "orpheus/Command/Render/CommandColor.hpp"
#include "orpheus/Material/MaterialFlatColor.hpp"

namespace Orpheus::Entity {
    class EntityRect : public Entity {
    private:
        float m_x;
        float m_y;
        std::shared_ptr<Command::Render::CommandMaterial<Material::MaterialFlatColor>> m_material;
        std::shared_ptr<Command::Render::CommandVertices> m_vertices;
        std::shared_ptr<Command::Render::CommandColor> m_color;

    public:
        EntityRect(float x, float y, float w, float h) :
            m_x(x),
            m_y(y),
            m_material(createRenderCommand<Command::Render::CommandMaterial<Material::MaterialFlatColor>>()),
            m_vertices(createRenderCommand<Command::Render::CommandVertices>()),
            m_color(createRenderCommand<Command::Render::CommandColor>(0.0f, 1.0f, 1.0f, 1.0f))
        {
            auto& positions = m_vertices->addAttrib(0, 2);
            positions->addPoint(0, 0);
            positions->addPoint(w, 0);
            positions->addPoint(w, h);
            positions->addPoint(w, h);
            positions->addPoint(0, 0);
            positions->addPoint(0, h);
        }

        virtual void draw(Render::Render& render) override {
            render.postRenderCommand(m_material);
            render.postRenderCommand(m_color);
            render.postRenderCommand(m_vertices);
        }
    };
}
