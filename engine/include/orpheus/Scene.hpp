#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"
#include "orpheus/Input/InputManager.hpp"
#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Entity/EntityCommand.hpp"
#include "orpheus/Render/Command/CommandClearColor.hpp"
#include "orpheus/Render/Command/CommandViewport.hpp"
#include <cmath>

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
        Math::Matrix4 m_projection;
        Math::Matrix4 m_view;

    protected:
        Math::Color& m_clearColor;

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
            m_projection = Math::Matrix4::perspective(static_cast<float>(M_PI / 3.0), 1.0f * m_width / m_height, 0.01f, 1000.0f);
        }

        void projectionOrtho() {
            float aspect = 1.0f * m_width / m_height;
            m_projection = Math::Matrix4::orthogonal(-aspect, aspect, -1.0f, 1.0f);
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

        const Math::Matrix4& getProjection() const {
            return m_projection;
        }

        const Math::Matrix4& getView() const {
            return m_view;
        }

        std::size_t getWidth() const {
            return m_width;
        }

        std::size_t getHeight() const {
            return m_height;
        }

        Math::Vector2 worldToScreen(float x, float y, float z) const {
            return m_projection.mul(m_view).mul(Math::Vector4(x, y, z, 1.0f)).getXY().add(1.0f, 1.0f).mul(0.5f * m_width, 0.5f * m_height);
        }

        Math::Vector3 screenToWorld(float x, float y) const {
            return m_projection.mul(m_view).inverse().mul(Math::Vector4(2.0f * x / m_width - 1.0f, 2.0f * y / m_height - 1.0f, 0.0f, 1.0f)).getXYZ();
        }

        Math::Vector2 getMouseCoordsScreen() const {
            return Math::Vector2(m_inputManager.getMouseX(), m_inputManager.getMouseY());
        }

        Math::Vector2 getMouseCoordsWorld() const {
            return screenToWorld(m_inputManager.getMouseX(), m_inputManager.getMouseY()).getXY();
        }
    };

    using ScenePtr = std::shared_ptr<Scene>;
}
