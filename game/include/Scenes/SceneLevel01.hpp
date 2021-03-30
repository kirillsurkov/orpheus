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
public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) : SceneLevel(sceneBase) {
        addScope("Level01");
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::CommandTest>("Level01 shown!");

        bindKeys();
    }

    virtual void update(float delta) override {
        m_clearColor->setR(std::fmod(m_clearColor->getR() + delta, 1.0f));
    }
};
