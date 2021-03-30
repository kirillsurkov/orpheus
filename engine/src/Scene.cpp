#include "orpheus/Scene.hpp"
#include "orpheus/Command/Render/RenderCommandClear.hpp"

Orpheus::Scene::Scene::Scene(Orpheus::Input::Manager& inputManager) :
    m_inputManager(inputManager)
{
    addScope("Scene");

    m_clearColorEntity = addEntity<Entity::EntityCommand<Command::Render::RenderCommandSetClearColor>>(0.0f, 0.0f, 0.0f, 1.0f);
    m_clearColor = m_clearColorEntity->getCommand();

    addEntity<Entity::EntityCommand<Command::Render::RenderCommandClear>>();
}

Orpheus::Scene::Scene::Scene(const Scene& scene) : Scene(scene.m_inputManager) {
    m_commandDispatcher = scene.m_commandDispatcher;
}
