#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"

namespace Orpheus::Window {
    class Window : public Loggable {
    private:
        TypeDispatcher m_createContextDispatcher;
        TypeDispatcher m_eventsDispatcher;

    protected:
        template<class T, class U>
        void registerContextType(U* self) {
            m_createContextDispatcher.registerType<T&>([self](T& ctx) { self->createContext(ctx); });
        }

        template<class T>
        void postEvent(const T& event) {
            if (!m_eventsDispatcher.dispatch(event)) {
                throw Exception(this, "Message of this type is not supported");
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
        void registerEventType(U* receiver) {
            m_eventsDispatcher.registerType<const std::shared_ptr<T>&>([receiver](const std::shared_ptr<T>& event) { receiver->onEvent(event); });
        }

    public:
        virtual void swapBuffers() = 0;
        virtual void pollEvents() = 0;
    };

    using WindowPtr = std::shared_ptr<Window>;
}
