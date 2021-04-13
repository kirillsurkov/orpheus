#include "Scenes/SceneLevel.hpp"

SceneLevel::SceneLevel(const Orpheus::Scene::Scene& sceneBase) : Scene(sceneBase) {
    addScope("Level");

    float aspect = 1.0f * getScreenHeight() / getScreenWidth();
    m_projection = glm::ortho(-1.0f, 1.0f, -aspect, aspect);

    m_roll = addEntity<EntityRoll>(3, 5);

    const float coverHeightScreen = 0.5f * (1.0f - m_fieldHeight) * getScreenHeight();
    const float coverHeight = screenToWorld(0.0f, coverHeightScreen).y - screenToWorld(0.0f, 0.0f).y;

    auto coverTopCoords = screenToWorld(0.0f, getScreenHeight() - coverHeightScreen);
    addEntity<Orpheus::Entity::Rect>(coverTopCoords.x, coverTopCoords.y, 2.0f, coverHeight)->getColor().set(0.1f, 0.0f, 0.0f, 1.0f);

    auto coverBotCoords = screenToWorld(0.0f, 0.0f);
    addEntity<Orpheus::Entity::Rect>(coverBotCoords.x, coverBotCoords.y, 2.0f, coverHeight)->getColor().set(0.1f, 0.0f, 0.0f, 1.0f);

    m_fpsIndicator = addEntity<Orpheus::Entity::Text>(0.0f, 0.0f, 32.0f, "FPS: ");
    auto fpsCoords = screenToWorld(0.0f, getScreenHeight() - m_fpsIndicator->getTextHeight());
    m_fpsIndicator->setX(fpsCoords.x);
    m_fpsIndicator->setY(fpsCoords.y);
    m_fpsIndicator->getColor().set(0.0f, 1.0f, 1.0f, 1.0f);

    m_lastResult = addEntity<Orpheus::Entity::Text>(0.0f, 0.0f, 32.0f, "");
    auto resCoords = screenToWorld(0.0f, getScreenHeight() - m_fpsIndicator->getTextHeight() - m_lastResult->getTextHeight());
    m_lastResult->setX(resCoords.x);
    m_lastResult->setY(resCoords.y);
    m_lastResult->getColor().set(1.0f, 1.0f, 0.0f, 1.0f);

    m_buttonSplit = addEntity<Orpheus::Entity::UI::Button>(0.0f, 0.0f, "SPLIT", [this]() {
        if (!m_roll->split()) {
            m_buttonSplit->setEnabled(false);
        }
    });
    m_buttonSplit->setEnabled(false);

    m_buttonStart = addEntity<Orpheus::Entity::UI::Button>(0.0f, 0.0f, "START", [this]() {
        m_buttonStart->setEnabled(false);
        m_buttonSplit->setEnabled(true);
        m_roll->start();
    });
}

void SceneLevel::onShow() {
    postCommand<Orpheus::Command::Game::CommandTest>("Level shown!");

    bindKey(Orpheus::Input::Key::ESC, [this](bool down) {
        if (down) {
            postCommand<Orpheus::Command::Game::CommandScenePop>();
        }
    });

    bindKey(Orpheus::Input::Key::LMB, [this](bool down) {
        m_buttonSplit->setMouseClicked(down);
        m_buttonStart->setMouseClicked(down);
    });
}

void SceneLevel::update(float delta) {
    m_fpsCounter++;
    m_fpsTimer += delta;
    if (m_fpsTimer >= m_fpsInterval) {
        m_fpsTimer -= m_fpsInterval;
        m_fpsIndicator->setText("FPS: " + std::to_string(static_cast<int>(m_fpsCounter / m_fpsInterval)));
        m_fpsCounter = 0;
    }

    auto coords = screenToWorld(getScreenWidth() - m_margin, m_margin);
    m_buttonStart->setX(coords.x - m_buttonStart->getWidth());
    m_buttonStart->setY(coords.y);

    float yScreen = worldToScreen(0.0f, m_buttonStart->getY() + m_buttonStart->getHeight(), 0.0f).y + m_margin;
    m_buttonSplit->setX(m_buttonStart->getX());
    m_buttonSplit->setY(screenToWorld(0.0f, yScreen).y);

    m_buttonSplit->setMousePos(getMouseX(), getMouseY());
    m_buttonStart->setMousePos(getMouseX(), getMouseY());

    auto worldLeftDown = screenToWorld(0.0f, 0.0f);
    float margin = screenToWorld(m_margin, 0.0f).x - worldLeftDown.x;
    m_roll->setX(worldLeftDown.x + margin);
    m_roll->setY(screenToWorld(0.0f, 0.5f * (1.0f - m_fieldHeight) * getScreenHeight()).y);
    m_roll->setWidth(screenToWorld(getScreenWidth(), 0.0f).x - worldLeftDown.x);
    m_roll->setHeight(screenToWorld(0.0f, m_fieldHeight * getScreenHeight()).y - worldLeftDown.y);
    m_roll->setMargin(margin);

    if (!m_buttonStart->isEnabled() && !m_roll->isSpinning()) {
        m_buttonStart->setEnabled(true);
        m_lastResult->setText("Last result: " + m_roll->getRow(1));
    }
}
