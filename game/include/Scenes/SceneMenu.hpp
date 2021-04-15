#pragma once

#include <orpheus/Scene.hpp>
#include <orpheus/Command/Game/CommandScenePop.hpp>
#include <orpheus/Command/Game/CommandTest.hpp>

class SceneMenu : public Orpheus::Scene::Scene {
protected:
    void bindKeys() {
        bindKey(Orpheus::Input::Key::ESC, [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::Game::CommandScenePop>();
            }
        });

        bindKey(Orpheus::Input::Key::W,   [this](bool down) { Orpheus::Log::info(this) << "Forward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::A,   [this](bool down) { Orpheus::Log::info(this) << "Left " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::S,   [this](bool down) { Orpheus::Log::info(this) << "Backward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::D,   [this](bool down) { Orpheus::Log::info(this) << "Right " << (down ? "down" : "up"); });

        bindKey(Orpheus::Input::Key::Z,   [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::Game::CommandTest>("It works from Menu!");
            }
        });
    }

public:
    SceneMenu(const Orpheus::Scene::Scene& sceneBase) : Orpheus::Scene::Scene(sceneBase) {
        addScope("Menu");

        float aspect = 1.0f * getScreenHeight() / getScreenWidth();

        m_projection = glm::ortho(-1.0f, 1.0f, -aspect, aspect);
    }
};
