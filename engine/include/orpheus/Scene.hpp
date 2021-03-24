#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"

namespace Orpheus::Scene {
    class Scene : public Loggable {
    private:
        EventsDispatcher m_eventsDispatcher;

    public:
        Scene() {
            addScope("Scene");
        }

        Scene(const Scene& scene) : Scene() {
            m_eventsDispatcher = scene.m_eventsDispatcher;
        }

        virtual ~Scene() {
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
    };

    using ScenePtr = std::shared_ptr<Scene>;
}
