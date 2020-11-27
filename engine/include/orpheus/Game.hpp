#pragma once

#include "orpheus/Engine.hpp"

namespace Orpheus {
    class Game : public Loggable {
    public:
        Game(const EnginePtr& engine);
        ~Game();
    };

    using GamePtr = std::shared_ptr<Game>;
}
