#include "orpheus/Scene.hpp"

#include <iostream>

namespace orpheus {
    Scene::Scene(const std::shared_ptr<orpheus::interface::IMath>& math,
                 const std::shared_ptr<orpheus::interface::IInput>& input,
                 const std::shared_ptr<orpheus::interface::IPhysics>& physics) :
        m_width(0),
        m_height(0),
        m_baseEntity(math, physics),
        m_input(input),
        m_physics(physics),
        m_math(math),
        m_camera(math)
    { }

    math::Vector2 Scene::getMouseDxDy() const {
        return m_input->getMouseDxDy();
    }

    physics::BodyID Scene::addBody(const physics::BodyDescription& description) {
        return m_physics->addBody(description);
    }

    render::SsboId Scene::createSSBO() {
        return m_ssboCounter++;
    }

    void Scene::setWidth(std::uint32_t width) {
        m_width = width;
    }

    void Scene::setHeight(std::uint32_t height) {
        m_height = height;
    }

    void Scene::draw(const std::shared_ptr<interface::IRender>& render) {
        render->setViewport(m_width, m_height);

        math::Matrix4x4 projection;
        m_math->perspective(projection, 3.141592653f * 1.0f / 3.0f, static_cast<float>(m_width) / m_height, 0.01f, 1000.0f);

        render->setProjection(projection);
        render->setView(m_camera.getTransform());
        render->setForward(m_camera.getForward());
        render->setRight(m_camera.getRight());

        for (const auto& entity : m_entities) {
            entity->draw(render);
        }
    }

    void Scene::update(float delta) {
        for (const auto& pair : m_input->getKeys()) {
            auto it = m_keyListeners.find(pair.first);
            if (it != m_keyListeners.end()) {
                it->second(pair.second);
            }
        }

        userUpdate(delta);

        auto mouseDelta = getMouseDxDy();
        m_math->mul(mouseDelta, mouseDelta, 0.01f);
        m_camera.update(mouseDelta, delta);

        for (const auto& entity : m_entities) {
            entity->update(delta);
        }
    }

    void Scene::userUpdate(float delta) {
    }
}
