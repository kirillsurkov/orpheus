#pragma once

#include "SceneLevel.hpp"

#include <orpheus/Command/Game/CommandTest.hpp>
#include <orpheus/Command/Game/CommandScenePush.hpp>
#include <orpheus/Command/Game/CommandScenePop.hpp>
#include <orpheus/Entity/EntityCommand.hpp>
#include <orpheus/Entity/EntityRect.hpp>
#include <orpheus/Entity/EntityCube.hpp>
#include <orpheus/Entity/EntityText.hpp>
#include <orpheus/Entity/UI/EntityButton.hpp>

#include <cmath>

class SceneLevel01 : public SceneLevel {
private:
    std::size_t m_fpsCounter = 0;
    float m_fpsTimer = 0.0f;
    const float m_fpsInterval = 0.2f;
    Orpheus::Entity::Text& m_fpsIndicator;
    Orpheus::Entity::Cube& m_cube;

public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) :
        SceneLevel(sceneBase),
        m_fpsIndicator(addEntity<Orpheus::Entity::Text>(0.0f, 0.0f, 16.0f, "FPS: ")),
        m_cube(addEntity<Orpheus::Entity::Cube>())
    {
        addScope("01");

        m_fpsIndicator.getColor().set(0.0f, 1.0f, 1.0f, 1.0f);
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::Game::CommandTest>("Level01 shown!");

        projectionPerspective();

        bindKeys();
    }

    virtual void update(float delta) override {
        m_fpsCounter++;
        m_fpsTimer += delta;
        if (m_fpsTimer >= m_fpsInterval) {
            m_fpsTimer -= m_fpsInterval;
            m_fpsIndicator.setText("FPS: " + std::to_string(static_cast<int>(m_fpsCounter / m_fpsInterval)));
            m_fpsCounter = 0;
        }

        auto topLeft = screenToWorld(0, getHeight());

        m_fpsIndicator.setX(topLeft.getX());
        m_fpsIndicator.setY(topLeft.getY() - m_fpsIndicator.getHeight());

        m_cube.rotate(delta);
    }
};
