#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"
#include "orpheus/InputManager.hpp"
#include "orpheus/Engine.hpp"
#include "orpheus/Entity/Entity.hpp"

namespace Orpheus::Scene {
    class Scene : public Loggable {
    private:
        CommandDispatcher m_commandDispatcher;
        Engine& m_engine;
        std::vector<Entity::EntityPtr> m_entities;

    protected:
        template<class T, class... Args>
        std::shared_ptr<T> addEntity(Args&&... args) {
            auto entity = std::make_shared<T>(std::forward<Args>(args)...);
            m_entities.push_back(entity);
            return entity;
        }

    public:
        Scene(Engine& engine) :
            m_engine(engine)
        {
            addScope("Scene");
        }

        Scene(const Scene& scene) : Scene(scene.m_engine) {
            m_commandDispatcher = scene.m_commandDispatcher;
        }

        virtual ~Scene() {
        }

        virtual void onShow() {
        }

        template<class T, class U>
        void registerCommand(U&& receiver) {
            m_commandDispatcher.registerCommand<T>(std::forward<U>(receiver));
        }

        template<class T, class... Args>
        void postCommand(Args&&... args) {
            auto command = std::make_shared<T>(std::forward<Args>(args)...);
            if (!m_commandDispatcher.dispatch(command)) {
                throw Exception(this, "Command '" + command->getName() + "' is not supported within Scene");
            }
        }

        template<class T>
        void bindKey(const Input::Key key, T&& function) {
            m_engine.bindKey(key, std::forward<T>(function));
        }

        const std::vector<Entity::EntityPtr>& getEntities() const {
            return m_entities;
        }

        void update(float delta) {
            for (const auto& entity : m_entities) {
                entity->update(delta);
            }
        }
    };

    using ScenePtr = std::shared_ptr<Scene>;
}
