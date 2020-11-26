#pragma once

#include <memory>

#include "orpheus/Scope.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus {
    class Render : public Scoped {
    public:
        class Context {
        public:
            virtual ~Context() = default;
        };

        static std::vector<std::string> getScopes() {
            return Utils::vectorAdd(Scoped::getScopes(), "Render");
        }

        using ContextPtr = std::shared_ptr<Context>;
    };

    using RenderPtr = std::shared_ptr<Render>;
}
