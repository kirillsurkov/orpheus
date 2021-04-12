#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"
#include "orpheus/Input/InputManager.hpp"
#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Entity/EntityCommand.hpp"
#include "orpheus/Render/Command/CommandClearColor.hpp"

#include <glm/ext.hpp>

namespace Orpheus::Scene {
    class Scene : public Loggable {
    private:
        CommandDispatcher m_commandDispatcher;
        std::size_t m_screenWidth;
        std::size_t m_screenHeight;
        Input::Manager& m_inputManager;
        Vertex::BufferCache& m_bufferCache;
        std::vector<Entity::EntityPtr> m_entities;
        std::shared_ptr<Entity::Command<Render::Command::ClearColor>> m_clearColorEntity;

    protected:
        glm::mat4x4 m_projection;
        glm::mat4x4 m_view;
        Render::Command::ClearColor& m_clearColor;

        template<class T, class... Args>
        std::shared_ptr<T> addEntity(Args&&... args) {
            try {
                auto entity = std::make_shared<T>(m_bufferCache, std::forward<Args>(args)...);
                m_entities.push_back(entity);
                return entity;
            } catch (const std::exception& e) {
                throw Exception(this, e.what());
            }
        }

    public:
        Scene(std::size_t screenWidth, std::size_t screenHeight, Input::Manager& inputManager, Vertex::BufferCache& bufferCache);
        Scene(const Scene& scene);

        virtual ~Scene() {}

        virtual void onShow() {}
        virtual void update(float/* delta*/) {}

        template<class T, class U>
        void registerCommand(U&& receiver) {
            m_commandDispatcher.registerCommand<T>(std::forward<U>(receiver));
        }

        template<class T, class... Args>
        void postCommand(Args&&... args) {
            m_commandDispatcher.dispatchOrThrow(this, T(std::forward<Args>(args)...));
        }

        template<class T>
        void bindKey(const Input::Key key, T&& function) {
            m_inputManager.bindKey(key, std::forward<T>(function));
        }

        const std::vector<Entity::EntityPtr>& getEntities() const {
            return m_entities;
        }

        const glm::mat4x4& getProjection() const {
            return m_projection;
        }

        const glm::mat4x4& getView() const {
            return m_view;
        }

        const std::size_t& getScreenWidth() const {
            return m_screenWidth;
        }

        const std::size_t& getScreenHeight() const {
            return m_screenHeight;
        }

        float getMouseX() const {
            float x = 2.0f * m_inputManager.getMouseX() / m_screenWidth - 1.0f;
            return (glm::inverse(m_projection * m_view) * glm::vec4(x, 0.0f, 0.0f, 1.0f)).x;
        }

        float getMouseY() const {
            float y = 2.0f * (1.0f - 1.0f * m_inputManager.getMouseY() / m_screenHeight) - 1.0f;
            return (glm::inverse(m_projection * m_view) * glm::vec4(0.0f, y, 0.0f, 1.0f)).y;
        }
    };

    using ScenePtr = std::shared_ptr<Scene>;
}
