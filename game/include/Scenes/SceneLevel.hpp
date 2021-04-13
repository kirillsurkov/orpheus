#pragma once

#include <orpheus/Scene.hpp>
#include <orpheus/Command/Game/CommandTest.hpp>
#include <orpheus/Command/Game/CommandScenePop.hpp>
#include <orpheus/Entity/UI/EntityButton.hpp>

#include "Entities/EntityRoll.hpp"

class SceneLevel : public Orpheus::Scene::Scene {
private:
    std::size_t m_fpsCounter  = 0;
          float m_fpsTimer    = 0.0f;
    const float m_fpsInterval = 0.2f;
    std::shared_ptr<Orpheus::Entity::Text> m_fpsIndicator;

    std::shared_ptr<Orpheus::Entity::Text> m_lastResult;

    const std::size_t m_margin = 10;
    std::shared_ptr<Orpheus::Entity::UI::Button> m_buttonSplit;
    std::shared_ptr<Orpheus::Entity::UI::Button> m_buttonStart;

    const float m_fieldHeight = 0.6f;
    std::shared_ptr<EntityRoll> m_roll;
    std::vector<float> m_speeds;

public:
    SceneLevel(const Orpheus::Scene::Scene& sceneBase);

    virtual void onShow() override;
    virtual void update(float delta) override;
};
