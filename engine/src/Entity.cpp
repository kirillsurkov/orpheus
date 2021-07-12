#include "orpheus/Entity.hpp"

namespace orpheus {
    Entity::Entity(const std::shared_ptr<orpheus::interface::IMath>& math,
                   const std::shared_ptr<orpheus::interface::IPhysics>& physics) :
        m_math(math),
        m_physics(physics)
    { }

    void Entity::draw(const std::shared_ptr<interface::IRender>& render) { }
    void Entity::update(float delta) { }
}
