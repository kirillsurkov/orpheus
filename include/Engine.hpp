#pragma once

#include "Window/Window.hpp"
#include "Render/Render.hpp"

class Engine {
private:
    WindowPtr m_window;
    RenderPtr m_render;

public:
    Engine(const WindowPtr& window, const RenderPtr& render);
    ~Engine();
    void loop();
};

using EnginePtr = std::shared_ptr<Engine>;
