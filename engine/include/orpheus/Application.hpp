#pragma once

#include "orpheus/Scope.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus {
    class Application : public Scoped {
    public:
        static std::vector<std::string> getScopes() {
            return Utils::vectorAdd(Scoped::getScopes(), "Application");
        }
    };
}
