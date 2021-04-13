#include "GameTest.hpp"
#include "Version.hpp"
#include "Scenes/SceneLevel.hpp"

GameTest::GameTest(const Orpheus::EnginePtr& engine) : Orpheus::Game(engine) {
    addScope("Test");
    Orpheus::Log::info(this) << "Version " << Version::Major << "." << Version::Minor;

    postCommand<Orpheus::Command::Game::CommandScenePush>(Orpheus::Utils::TypeIdentity<SceneLevel>{});
}

GameTest::~GameTest() {
    Orpheus::Log::info(this) << "Shut down";
}
