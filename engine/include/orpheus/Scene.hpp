#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"
#include "orpheus/Input/InputManager.hpp"
#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Entity/EntityCommand.hpp"
#include "orpheus/Render/Command/CommandClearColor.hpp"
#include "orpheus/Render/Command/CommandViewport.hpp"

#include <glm/ext.hpp>

namespace Orpheus::Scene {
    class Scene : public Loggable {
    private:
        CommandDispatcher m_commandDispatcher;
        std::size_t m_width;
        std::size_t m_height;
        Input::Manager& m_inputManager;
        Vertex::BufferCache& m_bufferCache;
        std::vector<std::unique_ptr<Entity::Entity>> m_entities;
        Entity::Command<Render::Command::ClearColor>& m_clearColorEntity;
        glm::mat4x4 m_projection;
        glm::mat4x4 m_view;

    protected:
        Render::Command::ClearColor& m_clearColor;

        template<class T, class... Args>
        T& addEntity(Args&&... args) {
            try {
                m_entities.emplace_back(std::make_unique<T>(m_bufferCache, std::forward<Args>(args)...));
                return *static_cast<T*>(m_entities.back().get());
            } catch (const std::exception& e) {
                throw Exception(this, e.what());
            }
        }

        virtual void onShow() {}
        virtual void update(float/* delta*/) {}

        void projectionPerspective() {
            m_projection = glm::perspective(static_cast<float>(M_PI / 3.0), 1.0f * m_width / m_height, 0.01f, 1000.0f);
        }

        void projectionOrtho() {
            float aspect = 1.0f * m_width / m_height;
            m_projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
        }

    public:
        Scene(std::size_t width, std::size_t height, Input::Manager& inputManager, Vertex::BufferCache& bufferCache);
        Scene(const Scene& scene);

        virtual ~Scene() {}

        void show() {
            onShow();
        }

        void step(float delta) {
            for (auto& entity : m_entities) {
                entity->update(delta);
            }
            update(delta);
        }

        void draw(Render::Render& render) {
            render.postCommand(Render::Command::Viewport(0, 0, m_width, m_height));
            for (auto& entity : m_entities) {
                entity->draw(m_projection, m_view, render);
            }
        }

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

        const glm::mat4x4& getProjection() const {
            return m_projection;
        }

        const glm::mat4x4& getView() const {
            return m_view;
        }

        const std::size_t& getWidth() const {
            return m_width;
        }

        const std::size_t& getHeight() const {
            return m_height;
        }

        glm::vec2 worldToScreen(float x, float y, float z) const {
            return 0.5f * (glm::vec2(m_projection * m_view * glm::vec4(x, y, z, 1.0f)) + 1.0f) * glm::vec2(m_width, m_height);
        }

        glm::vec3 screenToWorld(float x, float y) const {
            return glm::inverse(m_projection * m_view) * glm::vec4(2.0f * x / m_width - 1.0f, 2.0f * y / m_height - 1.0f, 0.0f, 1.0f);
        }

        float getMouseX() const {
            return screenToWorld(m_inputManager.getMouseX(), 0.0f).x;
        }

        float getMouseY() const {
            return screenToWorld(0.0f, m_inputManager.getMouseY()).y;
        }
    };

    using ScenePtr = std::shared_ptr<Scene>;
}
