#pragma once

#include "orpheus/Dispatcher.hpp"
#include "orpheus/Render/Render.hpp"

#include <vector>

namespace Orpheus::Entity {
    class Entity {
    protected:
        template<class T, class... Args>
        std::shared_ptr<T> createRenderCommand(Args&&... args) {
            return std::make_shared<T>(std::forward<Args>(args)...);;
        }

    public:
        virtual ~Entity() {}

        virtual void draw(Render::Render&/* render*/) {}
    };

    using EntityPtr = std::shared_ptr<Entity>;
}
