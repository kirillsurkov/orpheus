#pragma once

#include <orpheus/interface/IPhysics.hpp>

#include <unordered_map>

namespace orpheus::physics::bullet {
    class Bullet : public interface::IPhysics {
    private:
        std::unordered_map<interface::IPhysics::ID, interface::IPhysics::State> m_bodies;
        interface::IPhysics::ID                                                 m_idCounter;

    public:
        Bullet();

        virtual std::pair<interface::IPhysics::ID, interface::IPhysics::State> addBody(interface::IPhysics::Shape shape)                  override;
        virtual void                                                           update(float delta)                                        override;
        virtual void                                                           apply(const interface::IPhysics::Listener& listener) const override;
    };
}
