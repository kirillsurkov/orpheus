#include "GameTest.hpp"
#include "Version.hpp"
#include "Scenes/SceneLevel01.hpp"

GameTest::GameTest(Orpheus::Engine& engine) : Orpheus::Game(engine) {
    addScope("Test");
    Orpheus::Log::info(this) << "Version " << Version::Major << "." << Version::Minor;

    postCommand<Orpheus::Command::Game::CommandScenePush>(Orpheus::Utils::TypeIdentity<SceneLevel01>{});
}

GameTest::~GameTest() {
    Orpheus::Log::info(this) << "Shut down";
}
