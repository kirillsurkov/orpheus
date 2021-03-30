#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"
#include "orpheus/InputManager.hpp"
#include "orpheus/Entity/Entity.hpp"

namespace Orpheus::Scene {
    class Scene : public Loggable {
    private:
        CommandDispatcher m_commandDispatcher;
        Input::Manager& m_inputManager;
        std::vector<Entity::EntityPtr> m_entities;

    protected:
        template<class T, class... Args>
        std::shared_ptr<T> addEntity(Args&&... args) {
            auto entity = std::make_shared<T>(std::forward<Args>(args)...);
            m_entities.push_back(entity);
            return entity;
        }

    public:
        Scene(Input::Manager& inputManager);
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
            auto command = std::make_shared<T>(std::forward<Args>(args)...);
            if (!m_commandDispatcher.dispatch(command)) {
                throw Exception(this, "Command '" + command->getName() + "' is not supported within Scene");
            }
        }

        template<class T>
        void bindKey(const Input::Key key, T&& function) {
            m_inputManager.bindKey(key, std::forward<T>(function));
        }

        const std::vector<Entity::EntityPtr>& getEntities() const {
            return m_entities;
        }
    };

    using ScenePtr = std::shared_ptr<Scene>;
}
