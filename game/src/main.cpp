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

        auto window = std::make_shared<Orpheus::Window::SDL>("test", 800, 600);
        Orpheus::Log::info(window) << "Initialized in " << (timerPart.split() * 1e3) << "ms";

        auto render = std::make_shared<Orpheus::Render::OpenGL>(caches, window);
        Orpheus::Log::info(render) << "Initialized in " << (timerPart.split() * 1e3)  << "ms";

        auto engine = std::make_shared<Orpheus::Engine>(caches, window, render);
        Orpheus::Log::info(engine) << "Initialized in " << (timerPart.split() * 1e3)  << "ms";

        auto game = GameTest(engine);
        Orpheus::Log::info(game) << "Initialized in " << (timerTotal.split() * 1e3)  << "ms";

        game.run();
    /*} catch (const Orpheus::Exception& e) {
        Orpheus::Log::fatal(e);
    } catch (const std::exception& e) {
        Orpheus::Log::fatal() << e.what();
    }*/

    return 0;
}
