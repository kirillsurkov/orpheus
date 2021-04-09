#pragma once

#include "SceneLevel.hpp"

#include <orpheus/Command/Game/CommandTest.hpp>
#include <orpheus/Command/Game/CommandScenePush.hpp>
#include <orpheus/Command/Game/CommandScenePop.hpp>
#include <orpheus/Entity/EntityCommand.hpp>
#include <orpheus/Entity/EntityRect.hpp>
#include <orpheus/Entity/EntityText.hpp>

#include <cmath>

class SceneLevel01 : public SceneLevel {
public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) : SceneLevel(sceneBase) {
        addScope("01");

        addEntity<Orpheus::Entity::EntityRect>( 0.25f, 0.25f, 0.1f, 0.1f);
        addEntity<Orpheus::Entity::EntityRect>(-0.25f, 0.25f, 0.1f, 0.1f);
        addEntity<Orpheus::Entity::EntityText>(0.0f, 0.0f, 16.0f, "Test");
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::Game::CommandTest>("Level01 shown!");

        bindKeys();
    }

    virtual void update(float delta) override {
        //m_clearColor.setR(std::fmod(m_clearColor.getR() + delta, 1.0f));
    }
};
