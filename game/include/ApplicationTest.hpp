#pragma once

#include <orpheus/Application.hpp>
#include <orpheus/Window/Window.hpp>
#include <orpheus/Render/Render.hpp>
#include <orpheus/Engine.hpp>
#include <orpheus/Game.hpp>
#include <orpheus/Utils.hpp>

class ApplicationTest : public Orpheus::Application {
private:
    Orpheus::Window::WindowPtr m_window;
    Orpheus::Render::RenderPtr m_render;
    Orpheus::EnginePtr m_engine;
    Orpheus::GamePtr m_game;

public:
    ApplicationTest();
    ~ApplicationTest();

    virtual void run() override;
};
