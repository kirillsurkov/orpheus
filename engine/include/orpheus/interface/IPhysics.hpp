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

    struct CubeShapeInfo {
        float scaleX;
        float scaleY;
        float scaleZ;
    };

    struct SphereShapeInfo {
        float radius;
    };

    struct CapsuleShapeInfo {
        float radius;
        float height;
    };

    union ShapeInfo {
        CubeShapeInfo    cube;
        SphereShapeInfo  sphere;
        CapsuleShapeInfo capsule;
    };

    enum class Type {
        Static,
        Dynamic,
        Kinematic
    };

    struct BodyDescription {
        Shape         shape;
        ShapeInfo     shapeInfo;
        Type          type;
        math::Vector3 pos;
        math::Vector3 angularFactor = math::Vector3{1.0f, 1.0f, 1.0f};
        float         restitution = 1.0f;
    };

    struct GhostDescription {
        Shape         shape;
        ShapeInfo     shapeInfo;
        math::Vector3 pos;
    };

    struct State {
        math::Matrix4x4 transform;
    };

    using BodyID  = std::uint64_t;
}

namespace orpheus::interface {
    struct IPhysics {
        virtual void             init(float timestep)                                                    = 0;
        virtual physics::BodyID  addBody(const physics::BodyDescription& description)                    = 0;
        virtual void             update(float delta)                                                     = 0;
        virtual void             apply()                                                                 = 0;

        virtual void             setUserData(physics::BodyID id, std::uint64_t data)                     = 0;
        virtual std::uint64_t    getUserData(physics::BodyID id)                                   const = 0;

        virtual void             setLinearVelocity(physics::BodyID id, const math::Vector3& dir)         = 0;
        virtual math::Vector3    getLinearVelocity(physics::BodyID id)                             const = 0;

        virtual void             setPosition(physics::BodyID id, const math::Vector3& position)          = 0;
        virtual math::Vector3    getPosition(physics::BodyID id)                                   const = 0;

        virtual void             setRotation(physics::BodyID id, const math::Quaternion& rotation)       = 0;
        virtual math::Quaternion getRotation(physics::BodyID id)                                   const = 0;

        virtual void             setLinearFactor(physics::BodyID id, const math::Vector3& factor)        = 0;
        virtual void             setAngularFactor(physics::BodyID id, const math::Vector3& factor)       = 0;

        virtual physics::State   getState(physics::BodyID id)                                      const = 0;

        virtual const std::vector<physics::BodyID>& getCollisions(physics::BodyID id)              const = 0;
        virtual const std::vector<math::Vector3>&   getNormals(physics::BodyID id)                 const = 0;
    };
}
