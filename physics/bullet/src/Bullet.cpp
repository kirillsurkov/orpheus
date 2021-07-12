#include "physics/bullet/Bullet.hpp"

#include <iostream>

namespace orpheus::physics::bullet {
    Bullet::Bullet() :
        m_idCounter(0)
    {
        std::cout << "using bullet backend" << std::endl;
    }

    std::pair<interface::IPhysics::ID, interface::IPhysics::State> Bullet::addBody(interface::IPhysics::Shape shape) {
        return *m_bodies.emplace(m_idCounter++, interface::IPhysics::State{}).first;
    }

    void Bullet::update(float delta) {
    }

    void Bullet::apply(const interface::IPhysics::Listener& listener) const {
        for (const auto& pair : m_bodies) {
            listener(pair.first, pair.second);
        }
    }
}
