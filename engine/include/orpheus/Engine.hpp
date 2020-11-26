#pragma once

#include "orpheus/Scope.hpp"
#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/Render.hpp"
#include "orpheus/Utils.hpp"

namespace Orpheus {
    class Engine : public Scoped {
    private:
        WindowPtr m_window;
        RenderPtr m_render;

    public:
        static std::vector<std::string> getScopes() {
            return Utils::vectorAdd(Scoped::getScopes(), "Engine");
        }

        Engine(const WindowPtr& window, const RenderPtr& render);
        ~Engine();
        void loop();
    };

    using EnginePtr = std::shared_ptr<Engine>;
}
