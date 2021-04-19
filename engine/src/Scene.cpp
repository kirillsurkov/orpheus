#include "orpheus/Scene.hpp"
#include "orpheus/Render/Command/CommandClear.hpp"
#include "orpheus/Material/Command/CommandMatrixProjection.hpp"
#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Entity/EntityCommand.hpp"

Orpheus::Scene::Scene::Scene(std::size_t width, std::size_t height, Orpheus::Input::Manager& inputManager, Vertex::BufferCache& bufferCache) :
    m_width(width),
    m_height(height),
    m_inputManager(inputManager),
    m_bufferCache(bufferCache),
    m_clearColorEntity(addEntity<Entity::Command<Render::Command::ClearColor>>(0.0f, 0.0f, 0.0f, 1.0f)),
    m_clearColor(m_clearColorEntity.getCommand().getColor())
{
    addScope("Scene");

    addEntity<Entity::Command<Render::Command::Clear>>();
}

Orpheus::Scene::Scene::Scene(const Scene& scene) : Scene(scene.m_width, scene.m_height, scene.m_inputManager, scene.m_bufferCache) {
    m_commandDispatcher = scene.m_commandDispatcher;
}
