#include "GameTest.hpp"

#include <orpheus/Exception.hpp>

int main() {
    try {
        auto game = GameTest();
        game.run();
    } catch (const Orpheus::Exception& e) {
        Orpheus::Log::fatal(e);
    } catch (const std::exception& e) {
        Orpheus::Log::fatal() << e.what();
    }

    return 0;
}
