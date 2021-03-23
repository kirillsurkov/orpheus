#include "GameTest.hpp"

#include <orpheus/Log.hpp>
#include <orpheus/Utils.hpp>
#include <orpheus/Window/WindowGLFW.hpp>
#include <orpheus/Window/WindowSDL.hpp>
#include <orpheus/Render/RenderOpenGL.hpp>
#include <orpheus/Engine.hpp>

#include "Scenes/SceneMainMenu.hpp"
#include "Scenes/SceneLevel01.hpp"

GameTest::GameTest() {
    addScope("Test");

    auto timerTotal = Orpheus::Utils::StopWatch();
    auto timerPart = Orpheus::Utils::StopWatch();

    m_window = std::make_shared<Orpheus::Window::SDL>("test", 800, 600);
    Orpheus::Log::info(m_window) << "Initialized in " << timerPart.split() << "ms";

    m_render = std::make_shared<Orpheus::Render::OpenGL>();
    Orpheus::Log::info(m_render) << "Initialized in " << timerPart.split() << "ms";

    m_engine = std::make_shared<Orpheus::Engine>(m_window, m_render);
    Orpheus::Log::info(m_engine) << "Initialized in " << timerPart.split() << "ms";

    start();

    Orpheus::Log::info(this) << "Initialized in " << timerTotal.split() << "ms";
}

GameTest::~GameTest() {
    Orpheus::Log::info(this) << "Shut down";
}

void GameTest::start() {
    m_engine->bindKey(Orpheus::Input::Key::ESC, [this](bool/* down*/) { m_engine->postEvent<Orpheus::Event::EventQuit>(); });
    m_engine->bindKey(Orpheus::Input::Key::W,   [this](bool down) { Orpheus::Log::info(this) << "Forward " << (down ? "down" : "up"); });
    m_engine->bindKey(Orpheus::Input::Key::A,   [this](bool down) { Orpheus::Log::info(this) << "Left " << (down ? "down" : "up"); });
    m_engine->bindKey(Orpheus::Input::Key::S,   [this](bool down) { Orpheus::Log::info(this) << "Backward " << (down ? "down" : "up"); });
    m_engine->bindKey(Orpheus::Input::Key::D,   [this](bool down) { Orpheus::Log::info(this) << "Right " << (down ? "down" : "up"); });

    m_engine->postEvent<Orpheus::Event::EventLoadScene>(Orpheus::Utils::TypeIdentity<SceneLevel01>{});
}

bool GameTest::step() {
    //m_render->setClearColor(1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f);
    m_engine->step();
    return m_engine->isAlive();
}
