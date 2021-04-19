#pragma once

#include "orpheus/Dispatcher.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/Vertex.hpp"
#include "orpheus/Math/Matrix.hpp"

namespace Orpheus::Entity {
    class Entity {
    public:
        virtual ~Entity() {}

        virtual void update(float/* delta*/) {}

        virtual void draw(const Math::Matrix4&/* projection */,
                          const Math::Matrix4&/* view       */,
                          Render::Render&     /* render     */) const {}
    };

    using EntityPtr = std::shared_ptr<Entity>;
}
