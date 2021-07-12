#include "orpheus/Scene.hpp"

#include <iostream>

namespace orpheus {
    Scene::Scene(const std::shared_ptr<orpheus::interface::IMath>& math,
                 const std::shared_ptr<orpheus::interface::IPhysics>& physics) :
        m_width(0),
        m_height(0),
        m_baseEntity(math, physics),
        m_math(math),
        m_physics(physics)
    { }

    interface::IPhysics::State& Scene::addBody(interface::IPhysics::Shape shape) {
        return m_bodies.emplace(m_physics->addBody(shape)).first->second;
    }

    void Scene::setWidth(float width) {
        m_width = width;
    }

    void Scene::setHeight(float height) {
        m_height = height;
    }

    void Scene::setBody(interface::IPhysics::ID id, const interface::IPhysics::State& state) {
        m_bodies[id] = state;
    }

    void Scene::setKey(interface::IInput::Key key, const interface::IInput::State state) {
        std::cout << "key with id " << static_cast<int>(key) << ": " << static_cast<int>(state) << std::endl;
        m_keys[key] = state;
    }

    void Scene::draw(const std::shared_ptr<orpheus::interface::IRender>& render) {
        interface::math::Matrix4x4 projection;
        m_math->perspective(projection, 120.0f, m_width / m_height, 0.01f, 100.0f);
        render->setProjection(projection);
        for (const auto& entity : m_entities) {
            entity->draw(render);
        }
    }

    void Scene::update(float delta) {
        for (const auto& entity : m_entities) {
            entity->update(delta);
        }
    }
}
