#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/Event/EventQuit.hpp"
#include "orpheus/Event/EventKeyboard.hpp"
#include "orpheus/Event/EventMouse.hpp"
#include "orpheus/Event/EventLoadScene.hpp"
#include "orpheus/Scene.hpp"

namespace Orpheus {
    class Engine : public Loggable {
    private:
        Window::WindowPtr m_window;
        Render::RenderPtr m_render;
        std::vector<std::function<void()>> m_events;
        std::unordered_map<std::type_index, std::shared_ptr<Scene::Scene>> m_sceneCache;
        bool m_alive;

        Dispatcher<Event::EventKeyboard::Key> m_keysDownDispatcher;
        Dispatcher<Event::EventKeyboard::Key> m_keysUpDispatcher;

    public:
        Engine(const Window::WindowPtr& window, const Render::RenderPtr& render);
        ~Engine();

        bool isAlive() const;

        void loadScene(const std::shared_ptr<Scene::Scene>& scene);
        void step();

        template<class T, class... Args>
        void postEvent(Args&&... args) {
            auto event = std::make_shared<T>(args...);
            m_events.push_back([this, event]() {
                onEvent(event);
            });
        }

        template<class T>
        void bindKey(const Event::EventKeyboard::Key key, T&& function) {
            m_keysDownDispatcher.registerKey(key, [function]() { function(true); });
            m_keysUpDispatcher.registerKey(key,  [function]() { function(false); });
        }

        template<class T>
        void onEvent(const std::shared_ptr<Event::EventLoadScene<T>>&) {
            Log::info(this) << "loading scene";

            auto index = std::type_index(typeid(T));
            auto it = m_sceneCache.find(index);
            if (it == m_sceneCache.end()) {
                it = m_sceneCache.emplace(index, std::make_shared<T>()).first;
            }
            loadScene(it->second);
        }

        void onEvent(const std::shared_ptr<Event::EventQuit>& event);
        void onEvent(const std::shared_ptr<Event::EventKeyboard>& event);
        void onEvent(const std::shared_ptr<Event::EventMouse>& event);
    };

    using EnginePtr = std::shared_ptr<Engine>;
}
