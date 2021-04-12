#pragma once

#include "SceneMenuMain.hpp"

#include <orpheus/Scene.hpp>
#include <orpheus/Command/Game/CommandTest.hpp>
#include <orpheus/Command/Game/CommandScenePush.hpp>
#include <orpheus/Command/Game/CommandScenePop.hpp>

class SceneLevel : public Orpheus::Scene::Scene {
protected:
    void bindKeys() {
        bindKey(Orpheus::Input::Key::ESC, [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::Game::CommandScenePush>(Orpheus::Utils::TypeIdentity<SceneMenuMain>{});
            }
        });

        bindKey(Orpheus::Input::Key::W,   [this](bool down) { Orpheus::Log::info(this) << "Forward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::A,   [this](bool down) { Orpheus::Log::info(this) << "Left " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::S,   [this](bool down) { Orpheus::Log::info(this) << "Backward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::D,   [this](bool down) { Orpheus::Log::info(this) << "Right " << (down ? "down" : "up"); });

        bindKey(Orpheus::Input::Key::Z,   [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::Game::CommandTest>("It works from Level!");
            }
        });

        bindKey(Orpheus::Input::Key::X,   [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::Game::CommandScenePop>();
            }
        });
    }

public:
    SceneLevel(const Orpheus::Scene::Scene& sceneBase) : Orpheus::Scene::Scene(sceneBase) {
        addScope("Level");

        float aspect = 1.0f * getScreenHeight() / getScreenWidth();

        //m_projection = glm::perspective(static_cast<float>(M_PI / 3.0), 1.0f * getScreenWidth() / getScreenHeight(), 0.01f, 100.0f);
        m_projection = glm::ortho(-1.0f, 1.0f, -aspect, aspect);
    }
};
