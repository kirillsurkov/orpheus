#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/InputManager.hpp"
#include "orpheus/Command/CommandQuit.hpp"
#include "orpheus/Command/CommandKeyboard.hpp"
#include "orpheus/Command/CommandMouse.hpp"
#include "orpheus/Command/CommandScenePush.hpp"
#include "orpheus/Command/CommandScenePop.hpp"
#include "orpheus/Command/CommandTest.hpp"

#include <stack>

namespace Orpheus {
    class Engine : public Loggable {
    private:
        Window::WindowPtr m_window;
        Render::RenderPtr m_render;
        Input::Manager m_inputManager;
        std::unordered_map<std::type_index, std::shared_ptr<Scene::Scene>> m_sceneCache;
        std::stack<std::shared_ptr<Scene::Scene>> m_sceneStack;
        std::shared_ptr<Scene::Scene> m_sceneBase;
        bool m_alive;

        Dispatcher<Input::Key> m_keysDownDispatcher;
        Dispatcher<Input::Key> m_keysUpDispatcher;

        void pushScene(const std::shared_ptr<Scene::Scene>& scene);
        void popScene();

        void onCommand(const std::shared_ptr<Command::CommandQuit>& command);
        void onCommand(const std::shared_ptr<Command::CommandKeyboard>& command);
        void onCommand(const std::shared_ptr<Command::CommandMouse>& command);
        void onCommand(const std::shared_ptr<Command::CommandScenePush>& command);
        void onCommand(const std::shared_ptr<Command::CommandScenePop>& command);
        void onCommand(const std::shared_ptr<Command::CommandTest>& command);

    public:
        Engine(const Window::WindowPtr& window, const Render::RenderPtr& render);
        ~Engine();

        bool isAlive() const;

        void step();

        template<class T>
        void postCommand(T&& command) {
            if (m_alive) {
                onCommand(command);
            }
        }

        template<class T, class... Args>
        void postCommand(Args&&... args) {
            postCommand(std::make_shared<T>(std::forward<Args>(args)...));
        }

        template<class T>
        void bindKey(const Input::Key key, T&& function) {
            m_keysDownDispatcher.registerKey(key, [function]() { function(true); });
            m_keysUpDispatcher.registerKey(key,  [function]() { function(false); });
        }
    };

    using EnginePtr = std::shared_ptr<Engine>;
}
