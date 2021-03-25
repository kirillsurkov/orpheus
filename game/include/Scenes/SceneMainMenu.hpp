#pragma once

#include "SceneMenu.hpp"

#include <orpheus/Command/CommandScenePop.hpp>
#include <orpheus/Command/CommandTest.hpp>
#include <orpheus/Command/Render/RenderCommandClear.hpp>
#include <orpheus/Command/Render/RenderCommandSetClearColor.hpp>
#include <orpheus/Entity/EntityCommand.hpp>

class SceneMainMenu : public SceneMenu {
public:
    SceneMainMenu(const Orpheus::Scene::Scene& sceneBase) : SceneMenu(sceneBase) {
        addScope("Main");
        addEntity<Orpheus::Entity::EntityCommand<Orpheus::Command::Render::RenderCommandSetClearColor>>(0.0f, 0.5f, 0.5f, 1.0f);
        addEntity<Orpheus::Entity::EntityCommand<Orpheus::Command::Render::RenderCommandClear>>();
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::CommandTest>("MainMenu shown!");

        bindKeys();
    }
};
