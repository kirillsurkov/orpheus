#pragma once

#include "orpheus/Scope.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus {
    class Window : public Scoped {
    public:
        static std::vector<std::string> getScopes() {
            return Utils::vectorAdd(Scoped::getScopes(), "Window");
        }

        virtual Render::ContextPtr createContext() = 0;
        virtual void swapBuffers() = 0;
    };

    using WindowPtr = std::shared_ptr<Window>;
}
