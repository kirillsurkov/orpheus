#pragma once

#include "orpheus/Log.hpp"
#include "orpheus/Engine.hpp"

namespace Orpheus {
    class Game : public Loggable {
    private:
        Engine& m_engine;

    public:
        Game(Engine& engine);
        ~Game() = default;

        void run();

        template<class T, class... Args>
        void postCommand(Args&&... args) {
            m_engine.postCommand<T>(std::forward<Args>(args)...);
        }
    };
}
