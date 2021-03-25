#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Dispatcher.hpp"

namespace Orpheus::Window {
    class Window : public Loggable {
    private:
        TypeDispatcher m_createContextDispatcher;
        CommandDispatcher m_commandDispatcher;

    protected:
        template<class T, class U>
        void registerContextType(U* self) {
            m_createContextDispatcher.registerType<T&>([self](T& ctx) { self->createContext(ctx); });
        }

        template<class T>
        void postCommand(const std::shared_ptr<T>& command) {
            if (!m_commandDispatcher.dispatch(command)) {
                throw Exception(this, "Command '" + command->getName() + "' is not supported within Window");
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
        void registerCommand(U&& receiver) {
            m_commandDispatcher.registerCommand<T>(std::forward<U>(receiver));
        }

    public:
        virtual void swapBuffers() = 0;
        virtual void pollEvents() = 0;
    };

    using WindowPtr = std::shared_ptr<Window>;
}
