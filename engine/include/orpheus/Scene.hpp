#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"
#include "orpheus/InputManager.hpp"
#include "orpheus/Engine.hpp"

namespace Orpheus::Scene {
    class Scene : public Loggable {
    private:
        EventsDispatcher m_eventsDispatcher;
        Engine& m_engine;

    public:
        Scene(Engine& engine) :
            m_engine(engine)
        {
            addScope("Scene");
        }

        Scene(const Scene& scene) : Scene(scene.m_engine) {
            m_eventsDispatcher = scene.m_eventsDispatcher;
        }

        virtual ~Scene() {
        }

        virtual void onShow() {
        }

        template<class T, class U>
        void registerEventType(U&& receiver) {
            m_eventsDispatcher.registerEventType<T>(std::forward<U>(receiver));
        }

        template<class T, class... Args>
        void postEvent(Args&&... args) {
            auto event = std::make_shared<T>(std::forward<Args>(args)...);
            if (!m_eventsDispatcher.dispatch(event)) {
                throw Exception(this, "Event '" + event->getName() + "' is not supported within Scene");
            }
        }

        template<class T>
        void bindKey(const Input::Key key, T&& function) {
            m_engine.bindKey(key, std::forward<T>(function));
        }
    };

    using ScenePtr = std::shared_ptr<Scene>;
}
