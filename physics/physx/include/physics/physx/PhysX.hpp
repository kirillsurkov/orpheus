#pragma once

#include <orpheus/interface/IPhysics.hpp>

#include <PxPhysicsAPI.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>

#include <unordered_map>

namespace orpheus::physics::physx {
    class PhysX : public interface::IPhysics {
    private:
        float                                          m_timestep;
        float                                          m_timer;
        ID                                             m_idCounter;
        std::unordered_map<ID, ::physx::PxRigidActor*> m_actors;
        std::unordered_map<ID, ::physx::PxRigidBody*>  m_bodies;
        std::unordered_map<ID, ::physx::PxTransform>   m_statePrevTmp;
        std::unordered_map<ID, ::physx::PxTransform>   m_stateCurTmp;
        std::unordered_map<ID, ::physx::PxTransform>   m_statePrev;
        std::unordered_map<ID, ::physx::PxTransform>   m_stateCur;
        std::unordered_map<ID, Description>            m_bodiesToAdd;
        ::physx::PxDefaultAllocator                    m_pxAllocator;
        ::physx::PxDefaultErrorCallback                m_pxErrorCallback;
        ::physx::PxSimulationFilterShader              m_pxFilterShader;
        ::physx::PxFoundation*                         m_pxFoundation;
        ::physx::PxPhysics*                            m_pxPhysics;
        ::physx::PxScene*                              m_pxScene;

    public:
        PhysX();
        ~PhysX();

        virtual void          init(float timestep)                                     override;
        virtual ID            addBody(const Description& description)                  override;
        virtual void          update(float delta)                                      override;
        virtual void          apply()                                                  override;
        virtual void          setLinearVelocity(ID id, const math::Vector3& dir)       override;
        virtual math::Vector3 getLinearVelocity(ID id)                           const override;
        virtual State         getState(ID id)                                    const override;
    };
}
