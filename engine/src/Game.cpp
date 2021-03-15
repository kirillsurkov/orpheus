#include "orpheus/Game.hpp"

Orpheus::Game::Game(const EnginePtr& engine) :
    m_engine(engine)
{
    addScope("Game");
}

Orpheus::Game::~Game() {
}

void Orpheus::Game::run() {
    m_engine->loop();
}
