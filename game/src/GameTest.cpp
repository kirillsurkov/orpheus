#include "GameTest.hpp"
#include "Version.hpp"
#include "Scenes/SceneMainMenu.hpp"
#include "Scenes/SceneLevel01.hpp"

GameTest::GameTest(const Orpheus::EnginePtr& engine) : Orpheus::Game(engine) {
    addScope("Test");
    Orpheus::Log::info(this) << "Version " << Version::Major << "." << Version::Minor;

    bindKey(Orpheus::Input::Key::ESC, [this](bool/* down*/) { postEvent<Orpheus::Event::EventQuit>(); });
    bindKey(Orpheus::Input::Key::W,   [this](bool down) { Orpheus::Log::info(this) << "Forward " << (down ? "down" : "up"); });
    bindKey(Orpheus::Input::Key::A,   [this](bool down) { Orpheus::Log::info(this) << "Left " << (down ? "down" : "up"); });
    bindKey(Orpheus::Input::Key::S,   [this](bool down) { Orpheus::Log::info(this) << "Backward " << (down ? "down" : "up"); });
    bindKey(Orpheus::Input::Key::D,   [this](bool down) { Orpheus::Log::info(this) << "Right " << (down ? "down" : "up"); });

    postEvent<Orpheus::Event::EventLoadScene>(Orpheus::Utils::TypeIdentity<SceneLevel01>{});
}

GameTest::~GameTest() {
    Orpheus::Log::info(this) << "Shut down";
}
