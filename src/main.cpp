#include "Version.hpp"
#include "Log.hpp"
#include "Engine.hpp"
#include "Game.hpp"

void initEngine(EnginePtr& engine) {
    engine.reset();
}

int main() {
    Log::info("version") << Version::Major << "." << Version::Minor;

    EnginePtr engine;
    initEngine(engine);

    if (engine) {
        Game game(engine);
        engine->loop();
    }

    return 0;
}
