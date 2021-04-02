#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Command/Render/CommandMaterial.hpp"
#include "orpheus/Command/Render/CommandVertices.hpp"

namespace Orpheus::Entity {
    class EntityRect : public Entity {
    private:
        float m_x;
        float m_y;
        //std::shared_ptr<Command::Render::CommandMaterial> m_material;
        std::shared_ptr<Command::Render::CommandVertices> m_vertices;

    public:
        EntityRect(float x, float y, float w, float h) :
            m_x(x),
            m_y(y),
            //m_material(createRenderCommand<Command::Render::CommandMaterial>(Utils::TypeIdentity<Material::FlatColor>)),
            m_vertices(createRenderCommand<Command::Render::CommandVertices>())
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
            //render.postRenderCommand();
            render.postRenderCommand(m_vertices);
        }
    };
}
