#pragma once

#include "SceneLevel.hpp"

#include <orpheus/Command/CommandTest.hpp>
#include <orpheus/Command/CommandScenePush.hpp>
#include <orpheus/Command/CommandScenePop.hpp>
#include <orpheus/Command/Render/RenderCommandClear.hpp>
#include <orpheus/Command/Render/RenderCommandSetClearColor.hpp>
#include <orpheus/Entity/EntityCommand.hpp>

class SceneLevel01 : public SceneLevel {
public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) : SceneLevel(sceneBase) {
        addScope("Level01");
        addEntity<Orpheus::Entity::EntityCommand<Orpheus::Command::Render::RenderCommandSetClearColor>>(0.5f, 0.5f, 0.0f, 1.0f);
        addEntity<Orpheus::Entity::EntityCommand<Orpheus::Command::Render::RenderCommandClear>>();
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::CommandTest>("Level01 shown!");

        bindKeys();
    }
};
