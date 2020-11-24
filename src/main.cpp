#include "Log.hpp"
#include "Version.hpp"
#include "Window/WindowSDL.hpp"
#include "Render/RenderOpenGL.hpp"
#include "Engine.hpp"
#include "Game.hpp"

int main() {
    Log::info("version") << Version::Major << "." << Version::Minor;

    auto window = std::make_shared<WindowSDL::OpenGL>("test", 800, 600);
    auto render = std::make_shared<RenderOpenGL>(window->createContext());
    auto engine = std::make_shared<Engine>(window, render);
    auto game = std::make_shared<Game>(engine);
    engine->loop();

    return 0;
}
