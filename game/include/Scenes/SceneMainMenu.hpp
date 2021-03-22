#pragma once

#include <orpheus/Scene.hpp>

class SceneMainMenu : public Orpheus::Scene::Scene {
public:
    SceneMainMenu() {
        addScope("MainMenu");
    }
};
