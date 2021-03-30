#include "orpheus/Scene.hpp"

Orpheus::Scene::Scene::Scene(Orpheus::Input::Manager& inputManager) :
    m_inputManager(inputManager)
{
    addScope("Scene");
}

Orpheus::Scene::Scene::Scene(const Scene& scene) : Scene(scene.m_inputManager) {
    m_commandDispatcher = scene.m_commandDispatcher;
}
