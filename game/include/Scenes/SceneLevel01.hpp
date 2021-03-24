#pragma once

#include <orpheus/Scene.hpp>

#include <orpheus/Event/EventTest.hpp>

class SceneLevel01 : public Orpheus::Scene::Scene {
public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) : Orpheus::Scene::Scene(sceneBase) {
        addScope("Level01");
        postEvent<Orpheus::Event::EventTest>("It works!");
    }
};
