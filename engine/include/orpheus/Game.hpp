#pragma once

#include "orpheus/Engine.hpp"

namespace Orpheus {
    class Game : public Loggable {
    private:
        EnginePtr m_engine;

    public:
        Game(const EnginePtr& engine);
        ~Game();

        void run();
    };

    using GamePtr = std::shared_ptr<Game>;
}
