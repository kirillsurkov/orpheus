#pragma once

#include "SceneMenu.hpp"

#include <orpheus/Command/Game/CommandScenePop.hpp>
#include <orpheus/Command/Game/CommandTest.hpp>
#include <orpheus/Entity/EntityRect.hpp>

class SceneMenuMain : public SceneMenu {
private:
    float m_timer;
    std::shared_ptr<Orpheus::Entity::EntityRect> m_rect;

public:
    SceneMenuMain(const Orpheus::Scene::Scene& sceneBase) : SceneMenu(sceneBase) {
        addScope("Main");
        m_clearColor.set(0.0f, 0.0f, 0.0f, 1.0f);

        m_timer = 0.0f;
        m_rect = addEntity<Orpheus::Entity::EntityRect>(0.0f, 0.0f, 0.1f, 0.1f);
    }

    virtual void onShow() override {
        postCommand<Orpheus::Command::Game::CommandTest>("MenuMain shown!");

        bindKeys();
    }

    virtual void update(float delta) override {
        m_timer += delta;

        m_rect->setX(std::cos(m_timer));
        m_rect->setY(std::sin(m_timer));
    }
};
