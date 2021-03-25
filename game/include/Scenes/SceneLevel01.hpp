#pragma once

#include <orpheus/Scene.hpp>
#include <orpheus/Event/EventTest.hpp>
#include <orpheus/Event/EventScenePush.hpp>
#include <orpheus/Event/EventScenePop.hpp>

#include "SceneMainMenu.hpp"

class SceneLevel01 : public Orpheus::Scene::Scene {
public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) : Orpheus::Scene::Scene(sceneBase) {
        addScope("Level01");
    }

    virtual void onShow() override {
        postEvent<Orpheus::Event::EventTest>("Level01 shown!");

        bindKey(Orpheus::Input::Key::ESC, [this](bool down) {
            if (down) {
                postEvent<Orpheus::Event::EventScenePop>();
            }
        });

        bindKey(Orpheus::Input::Key::W,   [this](bool down) { Orpheus::Log::info(this) << "Forward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::A,   [this](bool down) { Orpheus::Log::info(this) << "Left " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::S,   [this](bool down) { Orpheus::Log::info(this) << "Backward " << (down ? "down" : "up"); });
        bindKey(Orpheus::Input::Key::D,   [this](bool down) { Orpheus::Log::info(this) << "Right " << (down ? "down" : "up"); });

        bindKey(Orpheus::Input::Key::Z,   [this](bool down) {
            if (down) {
                postEvent<Orpheus::Event::EventTest>("It works from Level01!");
            }
        });

        bindKey(Orpheus::Input::Key::X,   [this](bool down) {
            if (down) {
                postEvent<Orpheus::Event::EventScenePush>(Orpheus::Utils::TypeIdentity<SceneMainMenu>{});
            }
        });
    }
};
