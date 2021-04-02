#pragma once

#include "SceneMenu.hpp"

#include <orpheus/Command/Game/CommandScenePop.hpp>
#include <orpheus/Command/Game/CommandTest.hpp>
#include <orpheus/Entity/EntityCommand.hpp>

class SceneMainMenu : public SceneMenu {
public:
    SceneMainMenu(const Orpheus::Scene::Scene& sceneBase) : SceneMenu(sceneBase) {
        addScope("Main");
        m_clearColor->set(0.0f, 1.0f, 1.0f, 1.0f);
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::Game::CommandTest>("MainMenu shown!");

        bindKeys();
    }
};
