#include "orpheus/Scene.hpp"
#include "orpheus/Command/Render/CommandClear.hpp"
#include "orpheus/Command/Material/CommandMatrixProjection.hpp"
#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Entity/EntityCommand.hpp"

Orpheus::Scene::Scene::Scene(std::size_t screenWidth, std::size_t screenHeight, Orpheus::Input::Manager& inputManager) :
    m_screenWidth(screenWidth),
    m_screenHeight(screenHeight),
    m_inputManager(inputManager),
    m_clearColorEntity(addEntity<Entity::EntityCommand<Command::Render::CommandClearColor>>(0.0f, 0.0f, 0.0f, 1.0f)),
    m_projection(1.0f),
    m_view(1.0f),
    m_clearColor(m_clearColorEntity->getCommand())
{
    addScope("Scene");

    addEntity<Entity::EntityCommand<Command::Render::CommandClear>>();
}

Orpheus::Scene::Scene::Scene(const Scene& scene) : Scene(scene.m_screenWidth, scene.m_screenHeight, scene.m_inputManager) {
    m_commandDispatcher = scene.m_commandDispatcher;
}
