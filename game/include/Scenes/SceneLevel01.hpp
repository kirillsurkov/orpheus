#pragma once

#include <orpheus/Scene.hpp>

#include <orpheus/Event/EventTest.hpp>

class SceneLevel01 : public Orpheus::Scene::Scene {
public:
    SceneLevel01(Orpheus::Scene::Scene&& scene) : Orpheus::Scene::Scene(std::move(scene)) {
        addScope("Level01");
        postEvent<Orpheus::Event::EventTest>("it work!");
    }
};
