#pragma once

#include "orpheus/Entity.hpp"

#include "orpheus/interface/IInput.hpp"
#include "orpheus/interface/IMath.hpp"
#include "orpheus/interface/IPhysics.hpp"

#include <vector>
#include <unordered_map>

namespace orpheus {
    class Scene {
    private:
        float                                                                   m_width;
        float                                                                   m_height;
        Entity                                                                  m_baseEntity;
        std::shared_ptr<orpheus::interface::IMath>                              m_math;
        std::shared_ptr<orpheus::interface::IPhysics>                           m_physics;
        std::vector<std::shared_ptr<Entity>>                                    m_entities;
        std::unordered_map<interface::IPhysics::ID, interface::IPhysics::State> m_bodies;
        std::unordered_map<interface::IInput::Key, interface::IInput::State>    m_keys;

    public:
        Scene(const std::shared_ptr<orpheus::interface::IMath>& math,

              const std::shared_ptr<orpheus::interface::IPhysics>& physics);

        template<class T, class... Args>
        std::shared_ptr<T> addEntity(Args&&... args) {
            return std::static_pointer_cast<T>(m_entities.emplace_back(std::make_shared<T>(m_baseEntity, *this, std::forward<Args>(args)...)));
        }

        interface::IPhysics::State& addBody(interface::IPhysics::Shape shape);

        void setWidth(float width);
        void setHeight(float height);

        void setBody(interface::IPhysics::ID id, const interface::IPhysics::State& state);
        void setKey(interface::IInput::Key key, const interface::IInput::State state);

        void draw(const std::shared_ptr<orpheus::interface::IRender>& render);
        void update(float delta);
    };
}
