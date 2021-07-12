#include "physics/physx/PhysX.hpp"

#include <iostream>

namespace orpheus::physics::physx {
    PhysX::PhysX() :
        m_timestep(0),
        m_timer(0),
        m_idCounter(0)
    {
        std::cout << "using physx backend" << std::endl;
    }

    PhysX::~PhysX() {
        if (m_pxScene) {
            for (const auto& pair : m_actors) {
                m_pxScene->removeActor(*pair.second);
                pair.second->release();
            }
            m_pxScene->release();
        }

        if (m_pxPhysics) {
            m_pxPhysics->release();
        }
    }

    void PhysX::init(float timestep) {
        m_timestep = timestep;

        m_pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_pxAllocator, m_pxErrorCallback);
        m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, ::physx::PxTolerancesScale());

        if (!m_pxPhysics) {
            throw std::runtime_error("PxCreatePhysics failed");
        }

        if (!PxInitExtensions(*m_pxPhysics, nullptr)) {
            throw std::runtime_error("PxInitExtensions failed");
        }

        m_pxFilterShader = ::physx::PxDefaultSimulationFilterShader;

        ::physx::PxSceneDesc sceneDesc((::physx::PxTolerancesScale()));
        sceneDesc.gravity = ::physx::PxVec3(0.0f, -9.8f, 0.0f);
        if (!sceneDesc.cpuDispatcher) {
            ::physx::PxDefaultCpuDispatcher* cpuDispatcher = ::physx::PxDefaultCpuDispatcherCreate(1);
            if (!cpuDispatcher) {
                throw std::runtime_error("PxDefaultCpuDispatcherCreate failed");
            }
            sceneDesc.cpuDispatcher = cpuDispatcher;
        }
        if (!sceneDesc.filterShader) {
            sceneDesc.filterShader = m_pxFilterShader;
        }

        m_pxScene = m_pxPhysics->createScene(sceneDesc);
    }

    ID PhysX::addBody(const Description& description) {
        return m_bodiesToAdd.emplace(m_idCounter++, description).first->first;
    }

    void PhysX::update(float delta) {
        for (const auto& pair : m_bodiesToAdd) {
            const auto& description = pair.second;

            ::physx::PxTransform pose(::physx::PxVec3(description.pos.x(), description.pos.y(), description.pos.z()));
            ::physx::PxMaterial* material = m_pxPhysics->createMaterial(0.1f, 0.2f, 0.5f);
            ::physx::PxShape* shape;
            ::physx::PxRigidActor* actor;

            switch (description.shape) {
            case Shape::Cube: {
                shape = m_pxPhysics->createShape(::physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *material);
                break;
            }
            case Shape::Sphere: {
                shape = m_pxPhysics->createShape(::physx::PxSphereGeometry(1.0f), *material);
                break;
            }
            case Shape::Capsule: {
                shape = m_pxPhysics->createShape(::physx::PxCapsuleGeometry(1.0f, 1.0f), *material);
                shape->setLocalPose(::physx::PxTransform(::physx::PxQuat(::physx::PxHalfPi, ::physx::PxVec3(0.0f, 0.0f, 1.0f))));
                break;
            }
            }

            switch (description.type) {
            case Type::Static: {
                actor = m_pxPhysics->createRigidStatic(pose);
                actor->attachShape(*shape);
                break;
            }
            case Type::Dynamic: {
                ::physx::PxRigidDynamic* body = m_pxPhysics->createRigidDynamic(pose);
                body->attachShape(*shape);
                ::physx::PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
                m_bodies.emplace(pair.first, body);
                actor = body;
                break;
            }
            }

            m_pxScene->addActor(*actor);
            m_actors.emplace(pair.first, actor);
        }
        m_bodiesToAdd.clear();

        m_timer += delta;

        while (m_timer >= m_timestep) {
            m_pxScene->simulate(m_timestep);
            m_pxScene->fetchResults(true);
            m_timer -= m_timestep;

            std::swap(m_stateCurTmp, m_statePrevTmp);
            m_stateCurTmp.clear();
            for (const auto& pair : m_actors) {
                m_stateCurTmp.emplace(pair.first, pair.second->getGlobalPose());
            }
        }
    }

    void PhysX::apply() {
        m_statePrev = m_statePrevTmp;
        m_stateCur = m_stateCurTmp;
    }

    void PhysX::setLinearVelocity(ID id, const math::Vector3& dir) {
        auto it = m_bodies.find(id);
        if (it == m_bodies.end()) {
            throw std::runtime_error("Body with id " + std::to_string(id) + " not found");
        }
        it->second->setLinearVelocity(::physx::PxVec3(dir.x(), dir.y(), dir.z()));
    }

    math::Vector3 PhysX::getLinearVelocity(ID id) const {
        auto it = m_bodies.find(id);
        if (it == m_bodies.end()) {
            throw std::runtime_error("Body with id " + std::to_string(id) + " not found");
        }
        ::physx::PxVec3 velocity = it->second->getLinearVelocity();
        return math::Vector3{velocity.x, velocity.y, velocity.z};
    }

    State PhysX::getState(ID id) const {
        auto itPrev = m_statePrev.find(id);
        auto itCur = m_stateCur.find(id);

        ::physx::PxTransform transformPrev = (itPrev == m_statePrev.end() ? ::physx::PxTransform() : itPrev->second);
        ::physx::PxTransform transformCur = (itCur == m_stateCur.end() ? ::physx::PxTransform() : itCur->second);

        if (itPrev == m_statePrev.end() && itCur == m_stateCur.end()) {
            return State{math::Matrix4x4{{
                {0.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 0.0f}
            }}};
        }

        float alpha = m_timer / m_timestep;

        if (itCur == m_stateCur.end())   alpha = 0.0f;
        if (itPrev == m_statePrev.end()) alpha = 1.0f;

        auto mat33 = ::physx::PxMat33(transformCur.q * alpha + transformPrev.q * (1.0f - alpha));
        auto vec3 = transformCur.p * alpha + transformPrev.p * (1.0f - alpha);

        return State{math::Matrix4x4{{
            {mat33.column0[0], mat33.column0[1], mat33.column0[2], 0.0f},
            {mat33.column1[0], mat33.column1[1], mat33.column1[2], 0.0f},
            {mat33.column2[0], mat33.column2[1], mat33.column2[2], 0.0f},
            {vec3[0],          vec3[1],          vec3[2],          1.0f},
        }}};
    }
}
