#pragma once

#include <orpheus/interface/IPhysics.hpp>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace orpheus::physics::bullet {
	class Bullet : public interface::IPhysics {
    private:
        float                                                  m_timestep;
        float                                                  m_timer;
        BodyID                                                 m_bodyIdCounter;
        btDefaultCollisionConfiguration*                       m_collisionConfiguration;
        btCollisionDispatcher*                                 m_dispatcher;
        btBroadphaseInterface*                                 m_overlappingPairCache;
        btSequentialImpulseConstraintSolver*                   m_solver;
        btDiscreteDynamicsWorld*                               m_dynamicsWorld;
        btAlignedObjectArray<btCollisionShape*>                m_collisionShapes;
        std::unordered_map<BodyID, BodyDescription>            m_bodiesToAdd;
        std::unordered_map<BodyID, btRigidBody*>               m_bodies;
        std::unordered_map<BodyID, btTransform>                m_invTransform;
        std::unordered_map<BodyID, btTransform>                m_statePrevTmp;
        std::unordered_map<BodyID, btTransform>                m_stateCurTmp;
        std::unordered_map<BodyID, btTransform>                m_statePrev;
        std::unordered_map<BodyID, btTransform>                m_stateCur;
        std::unordered_map<BodyID, std::vector<BodyID>>        m_contactBodies;
        std::unordered_map<BodyID, std::vector<math::Vector3>> m_contactNormals;

        btRigidBody* getBody(BodyID id);
        const btRigidBody* getBody(BodyID id) const;

    public:
        Bullet();
        ~Bullet();

        virtual void              init(float timestep)                                                    override;
        virtual BodyID            addBody(const BodyDescription& description)                             override;
        virtual void              update(float delta)                                                     override;
        virtual void              apply()                                                                 override;

        virtual void              setUserData(BodyID id, std::uint64_t data)                              override;
        virtual std::uint64_t     getUserData(BodyID id)                                            const override;

        virtual void              setLinearVelocity(BodyID id, const math::Vector3& dir)                  override;
        virtual math::Vector3     getLinearVelocity(BodyID id)                                      const override;

        virtual void              setPosition(BodyID id, const math::Vector3& position)                   override;
        virtual math::Vector3     getPosition(BodyID id)                                            const override;

        virtual void              setRotation(physics::BodyID id, const math::Quaternion& rotation)       override;
        virtual math::Quaternion  getRotation(physics::BodyID id)                                   const override;

        virtual void              setAngularFactor(BodyID id, const math::Vector3& factor)                override;

        virtual State             getState(BodyID id)                                               const override;

        virtual const std::vector<BodyID>&        getCollisions(BodyID id)                          const override;
        virtual const std::vector<math::Vector3>& getNormals(BodyID id)                             const override;
    };
}
