#include "orpheus/Game.hpp"
#include "orpheus/Log.hpp"
#include "orpheus/Utils.hpp"

Orpheus::Game::Game(const EnginePtr& engine) :
    m_engine(engine)
{
    addScope("Game");
}

void Orpheus::Game::run() {
    Utils::StopWatch timer;
    while (m_engine->isAlive()) {
        m_engine->step(timer.split());
    }
}
