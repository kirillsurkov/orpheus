#pragma once

#include <orpheus/Scene.hpp>
#include <orpheus/Command/CommandScenePop.hpp>
#include <orpheus/Command/CommandTest.hpp>

class SceneMenu : public Orpheus::Scene::Scene {
protected:
    void bindKeys() {
        bindKey(Orpheus::Input::Key::ESC, [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::CommandScenePop>();
            }
        });

        bindKey(Orpheus::Input::Key::W,   [this](bool down) { Orpheus::Log::info(this) << "Forward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::A,   [this](bool down) { Orpheus::Log::info(this) << "Left " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::S,   [this](bool down) { Orpheus::Log::info(this) << "Backward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::D,   [this](bool down) { Orpheus::Log::info(this) << "Right " << (down ? "down" : "up"); });

        bindKey(Orpheus::Input::Key::Z,   [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::CommandTest>("It works from Menu!");
            }
        });
    }

public:
    SceneMenu(const Orpheus::Scene::Scene& sceneBase) : Orpheus::Scene::Scene(sceneBase) {
        addScope("Menu");
    }
};
