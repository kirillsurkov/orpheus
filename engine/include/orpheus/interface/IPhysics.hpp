#pragma once

#include "orpheus/interface/IMath.hpp"

#include <cstdint>
#include <functional>

namespace orpheus::physics {
    enum class Shape {
        Cube,
        Sphere,
        Capsule
    };

    enum class Type {
        Static,
        Dynamic
    };

    struct Description {
        Shape         shape;
        Type          type;
        math::Vector3 pos;
    };

    struct State {
        math::Matrix4x4 transform;
    };

    using ID = std::uint64_t;
}

namespace orpheus::interface {
    struct IPhysics {
        virtual void           init(float timestep)                                              = 0;
        virtual physics::ID    addBody(const physics::Description& description)                  = 0;
        virtual void           update(float delta)                                               = 0;
        virtual void           apply()                                                           = 0;
        virtual void           setLinearVelocity(physics::ID id, const math::Vector3& dir)       = 0;
        virtual math::Vector3  getLinearVelocity(physics::ID id)                           const = 0;
        virtual physics::State getState(physics::ID id)                                    const = 0;
    };
}
