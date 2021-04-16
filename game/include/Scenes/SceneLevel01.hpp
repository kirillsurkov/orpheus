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
    std::size_t m_fpsCounter = 0;
    float m_fpsTimer = 0.0f;
    const float m_fpsInterval = 0.2f;
    Orpheus::Entity::Text& m_fpsIndicator;
    std::vector<Orpheus::Entity::Text*> m_labels;

public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) :
        SceneLevel(sceneBase),
        m_fpsIndicator(addEntity<Orpheus::Entity::Text>(0.0f, 0.0f, 16.0f, "FPS: "))
    {
        addScope("01");

        m_fpsIndicator.getColor().set(0.0f, 1.0f, 1.0f, 1.0f);
        m_fpsIndicator.getAppearance().set(0.0f, 0.0f, 0.0f, 1.0f, 0.1f);

        const std::size_t cnt = 5;

        for (std::size_t i = 0; i < cnt; i++) {
            auto* label = &addEntity<Orpheus::Entity::Text>(0.0f, 0.0f, 12.0f + 72.0f * i / cnt, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", "bad-script");
            auto color = glm::rgbColor(glm::vec3(360.0f * i / cnt, 1.0f, 0.5f));
            label->getColor().set(color.x, color.y, color.z, 1.0f);
            auto color2 = glm::rgbColor(glm::vec3(360.0f * std::fmod(1.0f * i / cnt + 0.6f, 1.0f), 1.0f, 1.0f));
            label->getAppearance().set(color2.x, color2.y, color2.z, 1.0f, 4.0f);
            m_labels.push_back(label);
        }

        for (std::size_t i = 0; i < cnt; i++) {
            auto* label = &addEntity<Orpheus::Entity::Text>(0.0f, 0.0f, 12.0f + 72.0f * i / cnt, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", "ubuntu-mono");
            auto color = glm::rgbColor(glm::vec3(360.0f * i / cnt, 0.0f, 1.0f));
            label->getColor().set(color.x, color.y, color.z, 1.0f);
            auto color2 = glm::rgbColor(glm::vec3(360.0f * std::fmod(1.0f * i / cnt + 0.6f, 1.0f), 1.0f, 1.0f));
            label->getAppearance().set(color2.x, color2.y, color2.z, 0.0f, 0.0f);
            m_labels.push_back(label);
        }
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::Game::CommandTest>("Level01 shown!");

        projectionOrtho();

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

        auto leftRight = screenToWorld(0.0f, 0.0f);
        m_fpsIndicator.setX(leftRight.x);
        m_fpsIndicator.setY(screenToWorld(0.0f, getHeight()).y - m_fpsIndicator.getHeight());

        float textX = screenToWorld(10.0f, 0.0f).x;
        float textY = m_fpsIndicator.getY();

        for (auto* label : m_labels) {
            label->setX(textX);
            label->setY(textY -= label->getHeight());
        }
    }
};
