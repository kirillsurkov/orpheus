#include <orpheus/Log.hpp>
#include <orpheus/Utils.hpp>
#include <orpheus/Window/WindowSDL.hpp>
#include <orpheus/Render/RenderOpenGL.hpp>
#include <orpheus/Engine.hpp>
#include <orpheus/Game.hpp>

#include "Version.hpp"

int main() {
    /*try {
        auto app = ApplicationTest();
    } catch (const std::exception& e) {

    }

    app.run();

    return 0;*/

    //Orpheus::Log::info("Core") << "Version " << Version::Major << "." << Version::Minor;

    Orpheus::WindowPtr window;
    Orpheus::RenderPtr render;
    Orpheus::EnginePtr engine;
    Orpheus::GamePtr game;

    auto timerTotal = Orpheus::Utils::DeltaTime();
    auto timerPart = Orpheus::Utils::DeltaTime();

    try {
        window = std::make_shared<Orpheus::WindowSDL::OpenGL>("test", 800, 600);
        Orpheus::Log::info(window) << "Initialized in " << timerPart.getDelta() << " milliseconds";
    } catch (const std::exception& e) {
        Orpheus::Log::fatal(window) << e.what();
        return 0;
    }

    try {
        render = std::make_shared<Orpheus::RenderOpenGL>(window->createContext());
        Orpheus::Log::info(render) << "Initialized in " << timerPart.getDelta() << " milliseconds";
    } catch (const std::exception& e) {
        Orpheus::Log::fatal(render) << e.what();
        return 0;
    }

    try {
        engine = std::make_shared<Orpheus::Engine>(window, render);
        Orpheus::Log::info(engine) << "Initialized in " << timerPart.getDelta() << " milliseconds";
    } catch (const std::exception& e) {
        Orpheus::Log::fatal(engine) << e.what();
        return 0;
    }

    try {
        game = std::make_shared<Orpheus::Game>(engine);
        Orpheus::Log::info(game) << "Initialized in " << timerPart.getDelta() << " milliseconds";
    } catch (const std::exception& e) {
        Orpheus::Log::fatal(game) << e.what();
        return 0;
    }

    //Orpheus::Log::info("Core") << "Initialized in " << timerTotal.getDelta() << " milliseconds";

    engine->loop();

    return 0;
}
