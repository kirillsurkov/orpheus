#pragma once

#include "orpheus/Command/Game/Command.hpp"
#include "orpheus/Utils.hpp"
#include "orpheus/Scene.hpp"

#include <typeindex>
#include <functional>

namespace Orpheus::Command::Game {
    class CommandScenePush : public Command {
    private:
        std::type_index m_typeIndex;
        std::function<std::shared_ptr<Scene::Scene>(const std::shared_ptr<Scene::Scene>&)> m_createScene;

    public:
        template<class T, class... Args>
        CommandScenePush(Utils::TypeIdentity<T>, Args&&... args) :
            m_typeIndex(std::type_index(typeid(T))),
            m_createScene([args...](const std::shared_ptr<Scene::Scene>& sceneBase) {
                return std::make_shared<T>(*sceneBase, std::forward<Args>(args)...);
            })
        {
        }

        virtual std::string getName() const override {
            return "CommandScenePush";
        }

        std::type_index getTypeIndex() const {
            return m_typeIndex;
        }

        std::shared_ptr<Scene::Scene> createScene(const std::shared_ptr<Scene::Scene>& sceneBase) const {
            return m_createScene(sceneBase);
        }
    };
}
