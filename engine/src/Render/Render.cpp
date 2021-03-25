#include "orpheus/Render/Render.hpp"
#include "orpheus/Scene.hpp"

void Orpheus::Render::Render::init(Window::WindowPtr& window) {
   m_initializer(window);
}

void Orpheus::Render::Render::drawScene(const Orpheus::Scene::ScenePtr& scene) {
    for (const auto& entity : scene->getEntities()) {
        entity->draw(*this);
    }
}
