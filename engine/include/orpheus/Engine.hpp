#pragma once

#include "orpheus/Caches.hpp"
#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/Input/InputManager.hpp"
#include "orpheus/Command/Engine/CommandQuit.hpp"
#include "orpheus/Command/Game/CommandScenePush.hpp"
#include "orpheus/Command/Game/CommandScenePop.hpp"
#include "orpheus/Command/Game/CommandTest.hpp"

#include <stack>

namespace Orpheus {
    class Engine : public Loggable {
    private:
        Caches& m_caches;
        Window::WindowPtr m_window;
        Render::RenderPtr m_render;
        Input::Manager m_inputManager;
        std::unordered_map<std::type_index, std::shared_ptr<Scene::Scene>> m_sceneCache;
        std::stack<std::shared_ptr<Scene::Scene>> m_sceneStack;
        std::shared_ptr<Scene::Scene> m_sceneBase;
        bool m_alive;

        void onCommand(const Command::Engine::CommandQuit& command);
        void onCommand(const Command::Game::CommandScenePush& command);
        void onCommand(const Command::Game::CommandScenePop& command);
        void onCommand(const Command::Game::CommandTest& command);

    public:
        Engine(Caches& caches, const Window::WindowPtr& window, const Render::RenderPtr& render);
        ~Engine();

        bool isAlive() const;

        void step(float delta);

        template<class T>
        void postCommand(T&& command) {
            if (m_alive) {
                onCommand(command);
            }
        }

        template<class T, class... Args>
        void postCommand(Args&&... args) {
            postCommand(T(std::forward<Args>(args)...));
        }
    };

    using EnginePtr = std::shared_ptr<Engine>;
}
