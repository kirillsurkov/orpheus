#pragma once

#include "SceneMainMenu.hpp"

#include <orpheus/Scene.hpp>
#include <orpheus/Command/CommandTest.hpp>
#include <orpheus/Command/CommandScenePush.hpp>
#include <orpheus/Command/CommandScenePop.hpp>

class SceneLevel : public Orpheus::Scene::Scene {
protected:
    void bindKeys() {
        bindKey(Orpheus::Input::Key::ESC, [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::CommandScenePush>(Orpheus::Utils::TypeIdentity<SceneMainMenu>{});
            }
        });

        bindKey(Orpheus::Input::Key::W,   [this](bool down) { Orpheus::Log::info(this) << "Forward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::A,   [this](bool down) { Orpheus::Log::info(this) << "Left " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::S,   [this](bool down) { Orpheus::Log::info(this) << "Backward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::D,   [this](bool down) { Orpheus::Log::info(this) << "Right " << (down ? "down" : "up"); });

        bindKey(Orpheus::Input::Key::Z,   [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::CommandTest>("It works from Level!");
            }
        });

        bindKey(Orpheus::Input::Key::X,   [this](bool down) {
            if (down) {
                postCommand<Orpheus::Command::CommandScenePop>();
            }
        });
    }

public:
    SceneLevel(const Orpheus::Scene::Scene& sceneBase) : Orpheus::Scene::Scene(sceneBase) {
        addScope("Level01");
    }
};
