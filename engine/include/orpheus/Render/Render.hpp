#pragma once

#include "orpheus/Window/Window.hpp"

namespace Orpheus::Scene {
    class Scene;
}

namespace Orpheus::Render {
    class Render : public Loggable {
    public:
        class Context {
        public:
            virtual ~Context() = default;
        };

        using ContextPtr = std::shared_ptr<Context>;

    private:
        ContextPtr m_context;
        CommandDispatcher m_renderCommandDispatcher;

    protected:
        template<class T, class U>
        void registerCommand(U&& receiver) {
            m_renderCommandDispatcher.registerCommand<T>(std::forward<U>(receiver));
        }

    public:
        template<class T>
        Render(T*, const Window::WindowPtr& window) {
            addScope("Render");

            typename T::ContextPtr ctx;
            window->createContext(ctx);
            m_context = std::move(ctx);
        }

        template<class T>
        void postCommand(T&& command) {
            m_renderCommandDispatcher.dispatchOrThrow(this, command);
        }

        void drawScene(const std::shared_ptr<Orpheus::Scene::Scene>& scene);
    };

    using RenderPtr = std::shared_ptr<Render>;
}
