#pragma once

#include <orpheus/Game.hpp>
#include <orpheus/Window/Window.hpp>
#include <orpheus/Render/Render.hpp>
#include <orpheus/Engine.hpp>
#include <orpheus/Utils.hpp>

class GameTest : public Orpheus::Game {
private:
    Orpheus::Window::WindowPtr m_window;
    Orpheus::Render::RenderPtr m_render;
    Orpheus::EnginePtr m_engine;

    void start();

public:
    GameTest();
    ~GameTest();

    virtual bool step() override;
};
