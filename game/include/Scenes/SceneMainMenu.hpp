#pragma once

#include <orpheus/Scene.hpp>

class SceneMainMenu : public Orpheus::Scene::Scene {
public:
    SceneMainMenu(const Orpheus::Scene::Scene& sceneBase) : Orpheus::Scene::Scene(sceneBase) {
        addScope("MainMenu");
    }
};
