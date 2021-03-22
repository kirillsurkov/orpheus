#include "orpheus/Application.hpp"
#include "orpheus/Log.hpp"
#include "orpheus/Version.hpp"

Orpheus::Application::Application() {
    addScope("Application");
    Log::info(this) << "Version " << Version::Major << "." << Version::Minor;
}

void Orpheus::Application::run() {
    while (step());
}
