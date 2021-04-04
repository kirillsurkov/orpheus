#pragma once

#include "orpheus/Dispatcher.hpp"
#include "orpheus/Render/Render.hpp"

#include <vector>

namespace Orpheus::Entity {
    class Entity {
    public:
        virtual ~Entity() {}

        virtual void update(float/* delta*/) {}

        virtual void draw(Render::Render&/* render*/) {}
    };

    using EntityPtr = std::shared_ptr<Entity>;
}
