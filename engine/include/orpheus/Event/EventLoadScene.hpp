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
        std::function<std::shared_ptr<Scene::Scene>(Scene::Scene&&)> m_createScene;

    public:
        template<class T>
        EventLoadScene(Utils::TypeIdentity<T>) :
            m_typeIndex(std::type_index(typeid(T))),
            m_createScene([](Scene::Scene&& scene) {
                return std::make_shared<T>(std::move(scene));
            })
        {
        }

        virtual std::string getName() const override {
            return "EventLoadScene";
        }

        std::type_index getTypeIndex() const {
            return m_typeIndex;
        }

        std::shared_ptr<Scene::Scene> createScene(Scene::Scene&& scene) {
            return m_createScene(std::move(scene));
        }
    };
}
