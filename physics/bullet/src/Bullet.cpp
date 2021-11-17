#include "physics/bullet/Bullet.hpp"

#include <iostream>

namespace orpheus::physics::bullet {
    Bullet::Bullet() :
        m_timestep(1.0f / 60.0f),
        m_timer(0.0f),
        m_bodyIdCounter(0)
    {
        std::cout << "using bullet backend" << std::endl;
    }

    Bullet::~Bullet() {
        for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
            btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }
            m_dynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        for (int i = 0; i < m_collisionShapes.size(); i++) {
            btCollisionShape* shape = m_collisionShapes[i];
            m_collisionShapes[i] = 0;
            delete shape;
        }

        delete m_dynamicsWorld;
        delete m_solver;
        delete m_overlappingPairCache;
        delete m_dispatcher;
        delete m_collisionConfiguration;
    }

    void Bullet::init(float timestep) {
        m_timestep = timestep;

        m_collisionConfiguration = new btDefaultCollisionConfiguration();
        m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
        m_overlappingPairCache = new btDbvtBroadphase();
        m_solver = new btSequentialImpulseConstraintSolver;
        m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
        m_dynamicsWorld->setGravity(btVector3(0.0f, -50.0f, 0.0f));
    }

    BodyID Bullet::addBody(const BodyDescription& description) {
        return m_bodiesToAdd.emplace(m_bodyIdCounter++, description).first->first;
    }

    void Bullet::update(float delta) {
        for (const auto& pair : m_bodiesToAdd) {
            const auto& description = pair.second;

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(description.pos.x(), description.pos.y(), description.pos.z()));

            btTransform localTransform;
            localTransform.setIdentity();

            btCollisionShape* shape;

            switch (description.shape) {
            case Shape::Cube: {
                auto shapeInfo = description.shapeInfo.cube;
                shape = new btBoxShape(btVector3(shapeInfo.scaleX, shapeInfo.scaleY, shapeInfo.scaleZ));
                break;
            }
            case Shape::Sphere: {
                auto shapeInfo = description.shapeInfo.sphere;
                shape = new btSphereShape(shapeInfo.radius);
                break;
            }
            case Shape::Capsule: {
                auto shapeInfo = description.shapeInfo.capsule;
                shape = new btCapsuleShape(shapeInfo.radius, shapeInfo.height * 2);
                break;
            }
            }

            float mass = 0.0f;

            switch (description.type) {
            case Type::Static: {
                mass = 0.0f;
                break;
            }
            case Type::Dynamic: {
                mass = 1.0f;
                break;
            }
            case Type::Kinematic: {
                mass = 1.0f;
                break;
            }
            }

            btVector3 localIntertia(0.0f, 0.0f, 0.0f);

            if (description.type != Type::Static) {
                shape->calculateLocalInertia(1.0f, localIntertia);
            }

            btDefaultMotionState* motionState = new btDefaultMotionState(transform * localTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localIntertia);
            btRigidBody* body = new btRigidBody(rbInfo);
            body->setSleepingThresholds(0.0f, 0.0f);
            body->setRestitution(description.restitution);
            body->setAngularFactor(btVector3(description.angularFactor.x(), description.angularFactor.y(), description.angularFactor.z()));

            if (description.shape == Shape::Sphere) {
                body->setCcdMotionThreshold(1e-7);
                body->setCcdSweptSphereRadius(description.shapeInfo.sphere.radius);
            }

            if (description.type == Type::Kinematic) {
                body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
                body->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
                body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
                body->setActivationState(DISABLE_DEACTIVATION);
            }

            body->setUserIndex(pair.first);

            m_dynamicsWorld->addRigidBody(body);

            m_bodies.emplace(pair.first, body);
            m_invTransform.emplace(pair.first, localTransform.inverse());
            m_contactBodies.emplace(pair.first, std::vector<BodyID>());
            m_contactNormals.emplace(pair.first, std::vector<math::Vector3>());
        }
        m_bodiesToAdd.clear();

        m_timer += delta;

        while (m_timer >= m_timestep) {
            m_dynamicsWorld->stepSimulation(m_timestep, 1, m_timestep);

            m_timer -= m_timestep;

            std::swap(m_stateCurTmp, m_statePrevTmp);
            m_stateCurTmp.clear();
            for (const auto& pair : m_bodies) {
                m_stateCurTmp.emplace(pair.first, pair.second->getWorldTransform() * m_invTransform[pair.first]);
            }
        }

        for (const auto& pair : m_bodies) {
            m_contactBodies[pair.first].clear();
            m_contactNormals[pair.first].clear();
        }

        int manifolds = m_dispatcher->getNumManifolds();
        for (int i = 0; i < manifolds; i++) {
            const btPersistentManifold* manifold = m_dispatcher->getManifoldByIndexInternal(i);

            BodyID id1 = manifold->getBody0()->getUserIndex();
            BodyID id2 = manifold->getBody1()->getUserIndex();

            auto bodiesIt1 = m_contactBodies.find(id1);
            auto bodiesIt2 = m_contactBodies.find(id2);
            auto normalsIt1 = m_contactNormals.find(id1);
            auto normalsIt2 = m_contactNormals.find(id2);

            bodiesIt1->second.push_back(id2);
            bodiesIt2->second.push_back(id1);

            int contacts = manifold->getNumContacts();
            for (int j = 0; j < contacts; j++) {
                const btManifoldPoint& point = manifold->getContactPoint(j);
                normalsIt1->second.push_back(math::Vector3{-point.m_normalWorldOnB[0], -point.m_normalWorldOnB[1], -point.m_normalWorldOnB[2]});
                normalsIt2->second.push_back(math::Vector3{ point.m_normalWorldOnB[0],  point.m_normalWorldOnB[1],  point.m_normalWorldOnB[2]});
            }
        }
    }

    void Bullet::apply() {
        m_statePrev = m_statePrevTmp;
        m_stateCur = m_stateCurTmp;
    }

    btRigidBody* Bullet::getBody(BodyID id) {
        auto it = m_bodies.find(id);
        if (it == m_bodies.end()) {
            throw std::runtime_error("Body with id " + std::to_string(id) + " not found");
        }
        return it->second;
    }

    const btRigidBody* Bullet::getBody(BodyID id) const {
        auto it = m_bodies.find(id);
        if (it == m_bodies.end()) {
            throw std::runtime_error("Body with id " + std::to_string(id) + " not found");
        }
        return it->second;
    }

    void Bullet::setUserData(BodyID id, std::uint64_t data) {
        auto body = getBody(id);
        body->setUserIndex(data);
    }

    std::uint64_t Bullet::getUserData(BodyID id) const {
        auto body = getBody(id);
        return body->getUserIndex();
    }

    void Bullet::setLinearVelocity(BodyID id, const math::Vector3& dir) {
        auto body = getBody(id);
        body->setLinearVelocity(btVector3(dir.x(), dir.y(), dir.z()));
    }

    math::Vector3 Bullet::getLinearVelocity(BodyID id) const {
        auto body = getBody(id);
        btVector3 velocity = body->getLinearVelocity();
        return math::Vector3{velocity[0], velocity[1], velocity[2]};
    }

    void Bullet::setPosition(BodyID id, const math::Vector3& position) {
        auto body = getBody(id);

        btTransform transform;
        btMotionState* state = body->getMotionState();
        state->getWorldTransform(transform);
        transform.setOrigin(btVector3(position.x(), position.y(), position.z()));
        state->setWorldTransform(transform);

        body->setMotionState(state);

        /*btTransform transform = body->getWorldTransform();
        transform.setOrigin(btVector3(position.x(), position.y(), position.z()));
        body->setWorldTransform(transform);*/
    }

    math::Vector3 Bullet::getPosition(BodyID id) const {
        auto body = getBody(id);
        btTransform transform = body->getWorldTransform();
        btVector3 position = transform.getOrigin();
        return math::Vector3{position[0], position[1], position[2]};
    }

    void Bullet::setRotation(physics::BodyID id, const math::Quaternion& rotation) {
        auto body = getBody(id);

        btTransform transform;
        btMotionState* state = body->getMotionState();
        state->getWorldTransform(transform);
        transform.setRotation(btQuaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w()));
        state->setWorldTransform(transform);

        body->setMotionState(state);
    }

    math::Quaternion Bullet::getRotation(physics::BodyID id) const {
        auto body = getBody(id);
        btTransform transform = body->getWorldTransform();
        btQuaternion rotation = transform.getRotation();
        return math::Quaternion{rotation[0], rotation[1], rotation[2], rotation[3]};
    }

    void Bullet::setLinearFactor(BodyID id, const math::Vector3& factor) {
        auto body = getBody(id);
        body->setLinearFactor(btVector3(factor.x(), factor.y(), factor.z()));
    }

    void Bullet::setAngularFactor(BodyID id, const math::Vector3& factor) {
        auto body = getBody(id);
        body->setAngularFactor(btVector3(factor.x(), factor.y(), factor.z()));
    }

    State Bullet::getState(BodyID id) const {
        auto itPrev = m_statePrev.find(id);
        auto itCur = m_stateCur.find(id);

        btTransform transformPrev;
        if (itPrev == m_statePrev.end()) transformPrev.setIdentity();
        else transformPrev = itPrev->second;

        btTransform transformCur;
        if (itCur == m_stateCur.end()) transformCur.setIdentity();
        else transformCur = itCur->second;

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

        transformCur.getRotation() * alpha;
        auto mat33 = transformCur.getBasis()  * alpha + transformPrev.getBasis()  * (1.0f - alpha);
        auto vec3 =  transformCur.getOrigin() * alpha + transformPrev.getOrigin() * (1.0f - alpha);

        return State{math::Matrix4x4{{
            {mat33[0][0], mat33[1][0], mat33[2][0], 0.0f},
            {mat33[0][1], mat33[1][1], mat33[2][1], 0.0f},
            {mat33[0][2], mat33[1][2], mat33[2][2], 0.0f},
            {vec3[0],     vec3[1],     vec3[2],     1.0f},
        }}};
    }

    const std::vector<BodyID>& Bullet::getCollisions(BodyID id) const {
        auto it = m_contactBodies.find(id);
        if (it == m_contactBodies.end()) {
            throw std::runtime_error("Ghost(collisions) with id " + std::to_string(id) + " not found");
        }
        return it->second;
    }

    const std::vector<math::Vector3>& Bullet::getNormals(BodyID id) const {
        auto it = m_contactNormals.find(id);
        if (it == m_contactNormals.end()) {
            throw std::runtime_error("Ghost(normals) with id " + std::to_string(id) + " not found");
        }
        return it->second;
    }
}
