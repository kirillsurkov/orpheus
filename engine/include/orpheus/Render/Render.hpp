#pragma once

#include "orpheus/Window/Window.hpp"

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

        void init(Window::WindowPtr& window) {
            m_initializer(window);
        }

    public:
        virtual void setClearColor(float r, float g, float b, float a) = 0;
        virtual void clear() = 0;
    };

    using RenderPtr = std::shared_ptr<Render>;
}
