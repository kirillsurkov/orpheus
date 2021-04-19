#include <orpheus/Exception.hpp>
#include <orpheus/Caches.hpp>
#include <orpheus/Window/WindowSDL.hpp>
#include <orpheus/Render/RenderOpenGL.hpp>

#include "GameTest.hpp"

int main() {
    //try {
        auto timerTotal = Orpheus::Utils::StopWatch();
        auto timerPart = Orpheus::Utils::StopWatch();

        Orpheus::Caches caches;

        Orpheus::Window::SDL window("test", 800, 600);
        Orpheus::Log::info(window) << "Initialized in " << (timerPart.split() * 1e3) << "ms";

        Orpheus::Render::OpenGL render(caches, window);
        Orpheus::Log::info(render) << "Initialized in " << (timerPart.split() * 1e3)  << "ms";

        Orpheus::Engine engine(caches, window, render);
        Orpheus::Log::info(engine) << "Initialized in " << (timerPart.split() * 1e3)  << "ms";

        GameTest game(engine);
        Orpheus::Log::info(game) << "Initialized in " << (timerTotal.split() * 1e3)  << "ms";

        game.run();
    /*} catch (const Orpheus::Exception& e) {
        Orpheus::Log::fatal(e);
    } catch (const std::exception& e) {
        Orpheus::Log::fatal() << e.what();
    }*/

    return 0;
}
