#include "GameTest.hpp"
#include "Version.hpp"
#include "Scenes/SceneMainMenu.hpp"
#include "Scenes/SceneLevel01.hpp"

GameTest::GameTest(const Orpheus::EnginePtr& engine) : Orpheus::Game(engine) {
    addScope("Test");
    Orpheus::Log::info(this) << "Version " << Version::Major << "." << Version::Minor;

    postEvent<Orpheus::Event::EventScenePush>(Orpheus::Utils::TypeIdentity<SceneLevel01>{});
}

GameTest::~GameTest() {
    Orpheus::Log::info(this) << "Shut down";
}
