#pragma once

#include <orpheus/Scene.hpp>

class SceneLevel01 : public Orpheus::Scene::Scene {
public:
    SceneLevel01() {
        addScope("Level01");
    }
};
