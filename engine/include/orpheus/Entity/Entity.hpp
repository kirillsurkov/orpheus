#pragma once

#include "orpheus/Dispatcher.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/Vertex.hpp"

#include <glm/ext.hpp>

namespace Orpheus::Entity {
    class Entity {
    public:
        virtual ~Entity() {}

        virtual void update(float/* delta*/) {}

        virtual void draw(const glm::mat4x4&/* projection*/,
                          const glm::mat4x4&/* view*/,
                          Render::Render&   /* render*/) {}
    };

    using EntityPtr = std::shared_ptr<Entity>;
}
