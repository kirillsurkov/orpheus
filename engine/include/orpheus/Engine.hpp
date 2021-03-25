#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/InputManager.hpp"

#include <stack>

namespace Orpheus {
    namespace Scene {
        class Scene;
        using ScenePtr = std::shared_ptr<Scene>;
    }

    class Engine : public Loggable {
    private:
        Window::WindowPtr m_window;
        Render::RenderPtr m_render;
        Input::Manager m_inputManager;
        std::vector<std::function<bool()>> m_events;
        std::unordered_map<std::type_index, Scene::ScenePtr> m_sceneCache;
        std::stack<Scene::ScenePtr> m_sceneStack;
        Scene::ScenePtr m_sceneBase;
        bool m_alive;

        Dispatcher<Input::Key> m_keysDownDispatcher;
        Dispatcher<Input::Key> m_keysUpDispatcher;

    public:
        Engine(const Window::WindowPtr& window, const Render::RenderPtr& render);
        ~Engine();

        bool isAlive() const;

        void pushScene(const std::shared_ptr<Scene::Scene>& scene);
        bool popScene();
        void step();

        template<class T, class... Args>
        void postEvent(Args&&... args) {
            auto event = std::make_shared<T>(std::forward<Args>(args)...);
            m_events.push_back([this, event]() {
                return onEvent(event);
            });
        }

        template<class T>
        void bindKey(const Input::Key key, T&& function) {
            m_keysDownDispatcher.registerKey(key, [function]() { function(true); });
            m_keysUpDispatcher.registerKey(key,  [function]() { function(false); });
        }

        template<class T>
        bool onEvent(const std::shared_ptr<T>& event) {
            throw Exception(this, "Event '" + event->getName() + "' is not supported within Engine");
        }
    };

    using EnginePtr = std::shared_ptr<Engine>;
}
