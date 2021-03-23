#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"

namespace Orpheus::Window {
    class Window : public Loggable {
    private:
        TypeDispatcher m_createContextDispatcher;
        EventsDispatcher m_eventsDispatcher;

    protected:
        template<class T, class U>
        void registerContextType(U* self) {
            m_createContextDispatcher.registerType<T&>([self](T& ctx) { self->createContext(ctx); });
        }

        template<class T>
        void postEvent(const std::shared_ptr<T>& event) {
            if (!m_eventsDispatcher.dispatch(event)) {
                throw Exception(this, "Event '" + event->getName() + "' is not supported within Window");
            }
        }

    public:
        Window() {
            addScope("Window");
        }

        virtual ~Window() = default;

        template<class T>
        void createContext(T& ctx) {
            if (!m_createContextDispatcher.dispatch(ctx)) {
                throw Exception(this, "This render context is not supported");
            }
        }

        template<class T, class U>
        void registerEventType(U&& receiver) {
            m_eventsDispatcher.registerEventType<T>(std::forward<U>(receiver));
        }

    public:
        virtual void swapBuffers() = 0;
        virtual void pollEvents() = 0;
    };

    using WindowPtr = std::shared_ptr<Window>;
}
