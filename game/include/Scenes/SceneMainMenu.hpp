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
        m_clearColor->set(0.0f, 1.0f, 1.0f, 1.0f);
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::CommandTest>("MainMenu shown!");

        bindKeys();
    }
};
