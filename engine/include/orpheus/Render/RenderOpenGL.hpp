#pragma once

#include "orpheus/Render/Render.hpp"
#include "orpheus/Window/Window.hpp"
#include "orpheus/Utils.hpp"
#include "orpheus/Command/Render/RenderCommandClear.hpp"
#include "orpheus/Command/Render/RenderCommandSetClearColor.hpp"

namespace Orpheus::Render {
    class OpenGL : public Render {
    public:
        class Context : public Render::Context {};
        using ContextPtr = std::shared_ptr<Context>;

    private:
        void onCommand(const std::shared_ptr<Command::Render::RenderCommandClear>& command);
        void onCommand(const std::shared_ptr<Command::Render::RenderCommandSetClearColor>& command);

    public:
        OpenGL();
        ~OpenGL();

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
