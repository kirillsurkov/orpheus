#pragma once

#include <orpheus/Application.hpp>
#include <orpheus/Window/Window.hpp>
#include <orpheus/Render/Render.hpp>
#include <orpheus/Engine.hpp>
#include <orpheus/Utils.hpp>

class ApplicationTest : public Orpheus::Application {
private:
    Orpheus::Window::WindowPtr m_window;
    Orpheus::Render::RenderPtr m_render;
    Orpheus::EnginePtr m_engine;

    void init();

public:
    ApplicationTest();
    ~ApplicationTest();

    virtual bool step() override;
};
