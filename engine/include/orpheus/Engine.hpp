#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/Render.hpp"

namespace Orpheus {
    class Engine : public Loggable {
    private:
        Window::WindowPtr m_window;
        Render::RenderPtr m_render;

    public:
        Engine(const Window::WindowPtr& window, const Render::RenderPtr& render);
        ~Engine();

        void loop();
    };

    using EnginePtr = std::shared_ptr<Engine>;
}
