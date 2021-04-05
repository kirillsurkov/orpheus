#include "orpheus/Render/Render.hpp"
#include "orpheus/Scene.hpp"

void Orpheus::Render::Render::drawScene(const Orpheus::Scene::ScenePtr& scene) {
    const auto& projection = scene->getProjection();
    const auto& view = scene->getView();
    for (const auto& entity : scene->getEntities()) {
        entity->draw(projection, view, *this);
    }
}
