#include "orpheus/Game.hpp"
#include "orpheus/Log.hpp"

Orpheus::Game::Game(const EnginePtr& engine) :
    m_engine(engine)
{
    addScope("Game");
}

void Orpheus::Game::run() {
    while (m_engine->isAlive()) {
        m_engine->step();
    }
}
