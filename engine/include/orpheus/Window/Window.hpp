#pragma once

#include "orpheus/Render/Render.hpp"
#include "orpheus/Dispatcher.hpp"

namespace Orpheus::Window {
    class Window : public Loggable {
    private:
        Dispatcher m_createContextDispatcher;

    protected:
        template<class T, class U>
        void registerContextType(U* self) {
            m_createContextDispatcher.registerType<T&>([self](T& x) { self->createContext(x); });
        }

    public:
        Window() {
            addScope("Window");
        }

        virtual ~Window() = default;

        virtual void swapBuffers() = 0;

        template<class T>
        void createContext(T& t) {
            if (!m_createContextDispatcher.dispatch(t)) {
                throw Exception(this, "This render context is not supported");
            }
        }
    };

    using WindowPtr = std::shared_ptr<Window>;
}
