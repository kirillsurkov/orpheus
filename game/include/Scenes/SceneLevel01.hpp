#pragma once

#include "SceneLevel.hpp"

#include <orpheus/Command/CommandTest.hpp>
#include <orpheus/Command/CommandScenePush.hpp>
#include <orpheus/Command/CommandScenePop.hpp>
#include <orpheus/Command/Render/RenderCommandClear.hpp>
#include <orpheus/Command/Render/RenderCommandSetClearColor.hpp>
#include <orpheus/Entity/EntityCommand.hpp>

#include <cmath>

class SceneLevel01 : public SceneLevel {
private:
    std::shared_ptr<Orpheus::Entity::EntityCommand<Orpheus::Command::Render::RenderCommandSetClearColor>> m_clearColor;

public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) : SceneLevel(sceneBase) {
        addScope("Level01");
        m_clearColor = addEntity<Orpheus::Entity::EntityCommand<Orpheus::Command::Render::RenderCommandSetClearColor>>(0.5f, 0.5f, 0.0f, 1.0f);
        addEntity<Orpheus::Entity::EntityCommand<Orpheus::Command::Render::RenderCommandClear>>();
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::CommandTest>("Level01 shown!");

        bindKeys();
    }

    virtual void update(float delta) override {
        const auto& command = m_clearColor->getCommand();
        command->setR(std::fmod(command->getR() + delta, 1.0f));
    }
};
