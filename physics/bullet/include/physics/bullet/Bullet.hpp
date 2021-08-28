#pragma once

#include <orpheus/interface/IPhysics.hpp>

#include <unordered_map>

namespace orpheus::physics::bullet {
	class Bullet : public interface::IPhysics {
        virtual void          init(float timestep)                                     override;
        virtual ID            addBody(const Description& description)                  override;
        virtual void          update(float delta)                                      override;
        virtual void          apply()                                                  override;
        virtual void          setLinearVelocity(ID id, const math::Vector3& dir)       override;
        virtual math::Vector3 getLinearVelocity(ID id)                           const override;
        virtual State         getState(ID id)                                    const override;
    };
}
