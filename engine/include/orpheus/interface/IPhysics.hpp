#pragma once

#include <cstdint>
#include <functional>

namespace orpheus::interface {
    struct IPhysics {
        enum class Shape {
            Cube,
            Sphere
        };

        struct State {
        };

        using ID = std::uint64_t;

        using Listener = std::function<void(ID, const State&)>;

        virtual std::pair<ID, State> addBody(Shape shape)                  = 0;
        virtual void                 update(float delta)                   = 0;
        virtual void                 apply(const Listener& listener) const = 0;
    };
}
