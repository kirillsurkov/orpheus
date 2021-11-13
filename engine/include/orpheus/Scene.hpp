#pragma once

#include "orpheus/Camera.hpp"
#include "orpheus/Entity.hpp"

#include "orpheus/interface/IInput.hpp"
#include "orpheus/interface/IMath.hpp"
#include "orpheus/interface/IPhysics.hpp"

#include <vector>
#include <unordered_map>

namespace orpheus {
    class Scene {
    private:
        std::uint32_t                                                     m_width;
        std::uint32_t                                                     m_height;
        Entity                                                            m_baseEntity;
        std::shared_ptr<interface::IInput>                                m_input;
        std::shared_ptr<interface::IPhysics>                              m_physics;
        std::vector<std::shared_ptr<Entity>>                              m_entities;
        std::unordered_map<input::Key, std::function<void(input::State)>> m_keyListeners;
        std::uint32_t                                                     m_ssboCounter;

    protected:
        std::shared_ptr<interface::IMath> m_math;
        Camera                            m_camera;

    public:
        Scene(const std::shared_ptr<orpheus::interface::IMath>& math,
              const std::shared_ptr<orpheus::interface::IInput>& input,
              const std::shared_ptr<orpheus::interface::IPhysics>& physics);

        template<class T, class... Args>
        std::shared_ptr<T> addEntity(Args&&... args) {
            return std::static_pointer_cast<T>(m_entities.emplace_back(std::make_shared<T>(m_baseEntity, *this, std::forward<Args>(args)...)));
        }

        template<class T>
        void bindKey(input::Key key, T&& callback) {
            m_keyListeners[key] = callback;
        }

        void unbindKey(input::Key key) {
            m_keyListeners.erase(key);
        }

		math::Vector2 getMouseDxDy() const;

        physics::BodyID addBody(const physics::BodyDescription& description);

        render::SsboId createSSBO();

        void setWidth(std::uint32_t width);
        void setHeight(std::uint32_t height);

        void draw(const std::shared_ptr<interface::IRender>& render);
        void update(float delta);

        virtual void userUpdate(float delta);
    };
}
