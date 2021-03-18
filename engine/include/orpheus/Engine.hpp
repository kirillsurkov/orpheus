#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/Event/EventQuit.hpp"
#include "orpheus/Event/EventKeyboard.hpp"
#include "orpheus/Event/EventMouse.hpp"

namespace Orpheus {
    class Engine : public Loggable {
    private:
        Window::WindowPtr m_window;
        Render::RenderPtr m_render;
        bool m_alive;

        Dispatcher<Event::EventKeyboard::Key> m_keysDispatcher;

        void quit();

    public:
        Engine(const Window::WindowPtr& window, const Render::RenderPtr& render);
        ~Engine();

        void loop();

        template<class T>
        void bindKey(const Event::EventKeyboard::Key key, T&& function) {
            m_keysDispatcher.registerKey(key, function);
        }

        void onEvent(const std::shared_ptr<Event::EventQuit>& event);
        void onEvent(const std::shared_ptr<Event::EventKeyboard>& event);
        void onEvent(const std::shared_ptr<Event::EventMouse>& event);
    };

    using EnginePtr = std::shared_ptr<Engine>;
}
