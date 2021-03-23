#include "orpheus/Game.hpp"
#include "orpheus/Log.hpp"
#include "orpheus/Version.hpp"

Orpheus::Game::Game() {
    addScope("Application");
    Log::info(this) << "Version " << Version::Major << "." << Version::Minor;
}

void Orpheus::Game::run() {
    while (step());
}
