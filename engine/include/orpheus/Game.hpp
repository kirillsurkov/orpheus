#pragma once

#include "orpheus/Engine.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus {
    class Game : public Scoped {
    public:
        static std::vector<std::string> getScopes() {
            return Utils::vectorAdd(Scoped::getScopes(), "Game");
        }

        Game(const EnginePtr& engine);
        ~Game();
    };

    using GamePtr = std::shared_ptr<Game>;
}
