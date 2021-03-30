#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Engine.hpp"

namespace Orpheus {
    class Game : public Loggable {
    private:
        EnginePtr m_engine;

    public:
        Game(const EnginePtr& engine);
        ~Game() = default;

        void run();

        template<class T, class... Args>
        void postCommand(Args&&... args) {
            m_engine->postCommand<T>(std::forward<Args>(args)...);
        }
    };
}
