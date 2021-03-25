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
        std::function<void(Window::WindowPtr&)> m_initializer;
        ContextPtr m_context;
        CommandDispatcher m_renderCommandDispatcher;

    protected:
        template<class T, class U>
        void registerRenderCommand(U&& receiver) {
            m_renderCommandDispatcher.registerCommand<T>(std::forward<U>(receiver));
        }

    public:
        template<class T>
        Render(T*) {
            addScope("Render");

            m_initializer = [this](Window::WindowPtr& window) {
                typename T::ContextPtr ctx;
                window->createContext(ctx);
                m_context = std::move(ctx);
            };
        }

        template<class T>
        void postRenderCommand(T&& command) {
            if (!m_renderCommandDispatcher.dispatch(command)) {
                throw Exception(this, "Command '" + command->getName() + "' is not supported within Render");
            }
        }

        void init(Window::WindowPtr& window);
        void drawScene(const std::shared_ptr<Orpheus::Scene::Scene>& scene);
    };

    using RenderPtr = std::shared_ptr<Render>;
}
