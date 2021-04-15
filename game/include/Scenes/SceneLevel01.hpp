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
    Orpheus::Entity::UI::Button& m_button;

public:
    SceneLevel01(const Orpheus::Scene::Scene& sceneBase) :
        SceneLevel(sceneBase),
        m_fpsIndicator(addEntity<Orpheus::Entity::Text>(-1.0f, 0.667f, 32.0f, "FPS: ")),
        m_button(addEntity<Orpheus::Entity::UI::Button>(0.5f, -0.5f, "CLICK ME", [this]() {
            Orpheus::Log::info(this) << "Clicked";
        }))
    {
        addScope("01");

        m_fpsIndicator.getColor().set(0.0f, 1.0f, 1.0f, 1.0f);
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::Game::CommandTest>("Level01 shown!");

        bindKeys();

        bindKey(Orpheus::Input::Key::LMB, [this](bool down) {
            m_button.setMouseClicked(down);
        });
    }

    virtual void update(float delta) override {
        m_fpsCounter++;
        m_fpsTimer += delta;
        if (m_fpsTimer >= m_fpsInterval) {
            m_fpsTimer -= m_fpsInterval;
            m_fpsIndicator.setText("FPS: " + std::to_string(static_cast<int>(m_fpsCounter / m_fpsInterval)));
            m_fpsCounter = 0;
        }

        m_button.setMousePos(getMouseX(), getMouseY());
    }
};
