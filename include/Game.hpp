#pragma once

#include "Engine.hpp"

class Game {
public:
    Game(const EnginePtr& engine);
    ~Game();
};

using GamePtr = std::shared_ptr<Game>;
