#pragma once

#include "orpheus/Event/Event.hpp"
#include "orpheus/Scene.hpp"
#include "orpheus/Utils.hpp"

#include <typeindex>
#include <functional>

namespace Orpheus::Event {
    class EventLoadScene : public Event {
    private:
        std::type_index m_typeIndex;
        std::function<std::shared_ptr<Scene::Scene>(const Scene::ScenePtr&)> m_createScene;

    public:
        template<class T>
        EventLoadScene(Utils::TypeIdentity<T>) :
            m_typeIndex(std::type_index(typeid(T))),
            m_createScene([](const Scene::ScenePtr& sceneBase) {
                return std::make_shared<T>(*sceneBase);
            })
        {
        }

        virtual std::string getName() const override {
            return "EventLoadScene";
        }

        std::type_index getTypeIndex() const {
            return m_typeIndex;
        }

        std::shared_ptr<Scene::Scene> createScene(const Scene::ScenePtr& sceneBase) {
            return m_createScene(sceneBase);
        }
    };
}
