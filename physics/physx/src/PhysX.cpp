#include "physics/physx/PhysX.hpp"

#include <iostream>

namespace orpheus::physics::physx {
    PhysX::PhysX() :
        m_idCounter(0)
    {
        std::cout << "using physx backend" << std::endl;
    }

    std::pair<interface::IPhysics::ID, interface::IPhysics::State> PhysX::addBody(interface::IPhysics::Shape shape) {
        return *m_bodies.emplace(m_idCounter++, interface::IPhysics::State{}).first;
    }

    void PhysX::update(float delta) {
    }

    void PhysX::apply(const interface::IPhysics::Listener& listener) const {
        for (const auto& pair : m_bodies) {
            listener(pair.first, pair.second);
        }
    }
}
