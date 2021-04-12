#pragma once

#include "SceneLevel.hpp"

#include <orpheus/Command/Game/CommandTest.hpp>
#include <orpheus/Command/Game/CommandScenePush.hpp>
#include <orpheus/Command/Game/CommandScenePop.hpp>
#include <orpheus/Entity/EntityCommand.hpp>
#include <orpheus/Entity/EntityRect.hpp>
#include <orpheus/Entity/EntityText.hpp>
#include <orpheus/Entity/UI/EntityButton.hpp>

#include <cmath>

class SceneLevel01 : public SceneLevel {
private:
    std::size_t m_fpsCounter  = 0;
          float m_fpsTimer    = 0.0f;
    const float m_fpsInterval = 0.2f;
    std::shared_ptr<Orpheus::Entity::Text> m_fpsIndicator;

    const std::size_t m_margin = 10;
    std::shared_ptr<Orpheus::Entity::UI::Button> m_buttonSplit;
    std::shared_ptr<Orpheus::Entity::UI::Button> m_buttonStart;

    const std::size_t m_rowSize = 5;
    const std::size_t m_colSize = 3;
    const float m_fieldHeight = 0.7f;

    std::vector<std::vector<std::shared_ptr<Orpheus::Entity::Rect>>> m_rolls;
    std::vector<float> m_speeds;

public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) : SceneLevel(sceneBase) {
        addScope("01");

        float xStep = 1.0f * (getScreenWidth() - m_margin) / m_rowSize;
        float widthWorld = screenToWorld(glm::vec2(xStep, 0.0f)).x - screenToWorld(glm::vec2(m_margin, 0.0f)).x;
        float yStep = m_fieldHeight * (getScreenHeight() - m_margin) / m_colSize;
        float heightWorld = screenToWorld(glm::vec2(0.0f, m_margin)).y - screenToWorld(glm::vec2(0.0f, yStep)).y;
        for (std::size_t x = 0; x < m_rowSize; x++) {
            float xWorld = screenToWorld(glm::vec2(x * xStep + m_margin, 0.0f)).x;
            std::vector<std::shared_ptr<Orpheus::Entity::Rect>> roll;
            for (std::size_t y = 0; y < m_colSize; y++) {
                float yWorld = screenToWorld(glm::vec2(0.0f, 0.5f * (1.0f - m_fieldHeight) * getScreenHeight() + y * yStep + m_margin)).y;
                roll.push_back(addEntity<Orpheus::Entity::Rect>(xWorld, yWorld, widthWorld, heightWorld));
            }
            m_rolls.push_back(roll);
            m_speeds.push_back(0.5f + 0.5f * rand() / static_cast<float>(RAND_MAX));
        }

        auto coverTopCoords = screenToWorld(glm::vec2(0.0f, 0.0f));
        auto coverTop = addEntity<Orpheus::Entity::Rect>(coverTopCoords.x, coverTopCoords.y, 2.0f, -0.3f);
        coverTop->getColor().set(0.0f, 0.0f, 0.0f, 1.0f);
        auto coverBotCoords = screenToWorld(glm::vec2(0.0f, getScreenHeight()));
        auto coverBot = addEntity<Orpheus::Entity::Rect>(coverBotCoords.x, coverBotCoords.y, 2.0f, 0.5f);
        coverBot->getColor().set(0.0f, 0.0f, 0.0f, 1.0f);

        m_fpsIndicator = addEntity<Orpheus::Entity::Text>(0.0f, 0.0f, 32.0f, "FPS: ");
        auto coords = screenToWorld(glm::vec2(0.0f, m_fpsIndicator->getTextHeight()));
        m_fpsIndicator->setX(coords.x);
        m_fpsIndicator->setY(coords.y);
        m_fpsIndicator->getColor().set(0.0f, 1.0f, 1.0f, 1.0f);

        m_buttonSplit = addEntity<Orpheus::Entity::UI::Button>(0.0f, 0.0f, "SPLIT");
        m_buttonStart = addEntity<Orpheus::Entity::UI::Button>(0.0f, 0.0f, "START");
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::Game::CommandTest>("Level01 shown!");

        bindKeys();

        bindKey(Orpheus::Input::Key::LMB, [this](bool down) {
            m_buttonSplit->setMouseClicked(down);
            m_buttonStart->setMouseClicked(down);
        });
    }

    virtual void update(float delta) override {
        m_fpsCounter++;
        m_fpsTimer += delta;
        if (m_fpsTimer >= m_fpsInterval) {
            m_fpsTimer -= m_fpsInterval;
            m_fpsIndicator->setText("FPS: " + std::to_string(static_cast<int>(m_fpsCounter / m_fpsInterval)));
            m_fpsCounter = 0;
        }

        auto coords = screenToWorld(glm::vec2(getScreenWidth() - m_margin, getScreenHeight() - m_margin));
        m_buttonStart->setX(coords.x - m_buttonStart->getWidth());
        m_buttonStart->setY(coords.y);

        float yScreen = worldToScreen(glm::vec3(0.0f, m_buttonStart->getY() + m_buttonStart->getHeight(), 0.0f)).y;
        m_buttonSplit->setX(m_buttonStart->getX());
        m_buttonSplit->setY(screenToWorld(glm::vec2(0.0f, yScreen - m_margin)).y);

        m_buttonSplit->setMousePos(getMouseX(), getMouseY());
        m_buttonStart->setMousePos(getMouseX(), getMouseY());

        //0.5 * x * (1 + a)

        //float yWorld = screenToWorld(glm::vec2(0.0f, 0.5f * getScreenHeight() * (1.0f - m_fieldHeight) + m_margin)).y;
        float moveFrom = (1.0f - 0.5f * m_fieldHeight) * screenToWorld(glm::vec2(0.0f, getScreenHeight())).y - 0.1f;
        float fieldHeightWorld = -screenToWorld(glm::vec2(0.0f, getScreenHeight() - m_margin)).y;// * m_fieldHeight;
        for (std::size_t i = 0; i < m_rolls.size(); i++) {
            for (auto& cell : m_rolls[i]) {
                float y = cell->getY() - m_speeds[i] * delta;
                if (y <= moveFrom) {
                    y += fieldHeightWorld / m_fieldHeight;
                }
                cell->setY(y);
            }
        }
    }
};
