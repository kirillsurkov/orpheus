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
        void postEvent(Args&&... args) {
            m_engine->postEvent<T>(std::forward<Args>(args)...);
        }

        template<class T>
        void bindKey(const Input::Key key, T&& function) {
            m_engine->bindKey(key, std::forward<T>(function));
        }
    };
}
