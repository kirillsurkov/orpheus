#include "ApplicationTest.hpp"

#include <orpheus/Log.hpp>
#include <orpheus/Utils.hpp>
#include <orpheus/Window/WindowGLFW.hpp>
#include <orpheus/Window/WindowSDL.hpp>
#include <orpheus/Render/RenderOpenGL.hpp>
#include <orpheus/Engine.hpp>
#include <orpheus/Game.hpp>

ApplicationTest::ApplicationTest() {
    addScope("Test");

    auto timerTotal = Orpheus::Utils::DeltaTime();
    auto timerPart = Orpheus::Utils::DeltaTime();

    m_window = std::make_shared<Orpheus::Window::SDL>("test", 800, 600);
    Orpheus::Log::info(m_window) << "Initialized in " << timerPart.getDelta() << " milliseconds";

    m_render = std::make_shared<Orpheus::Render::OpenGL>(m_window);
    Orpheus::Log::info(m_render) << "Initialized in " << timerPart.getDelta() << " milliseconds";

    m_engine = std::make_shared<Orpheus::Engine>(m_window, m_render);
    Orpheus::Log::info(m_engine) << "Initialized in " << timerPart.getDelta() << " milliseconds";

    m_game = std::make_shared<Orpheus::Game>(m_engine);
    Orpheus::Log::info(m_game) << "Initialized in " << timerPart.getDelta() << " milliseconds";

    Orpheus::Log::info(this) << "Initialized in " << timerTotal.getDelta() << " milliseconds";
}

ApplicationTest::~ApplicationTest() {
    Orpheus::Log::info(this) << "Shut down";
}

void ApplicationTest::run() {
    m_engine->loop();
}
