#pragma once

#include "orpheus/interface/IMath.hpp"
#include "orpheus/interface/IPhysics.hpp"
#include "orpheus/interface/IRender.hpp"

namespace orpheus {
    class Entity {
    protected:
        std::shared_ptr<orpheus::interface::IMath>    m_math;
        std::shared_ptr<orpheus::interface::IPhysics> m_physics;

    public:
        Entity(const std::shared_ptr<orpheus::interface::IMath>& math,
               const std::shared_ptr<orpheus::interface::IPhysics>& physics);

        virtual void update(float delta);
        virtual void draw(const std::shared_ptr<interface::IRender>& render);
    };
}
