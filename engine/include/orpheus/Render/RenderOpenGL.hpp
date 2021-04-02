#pragma once

#include "orpheus/Render/Render.hpp"
#include "orpheus/Command/Render/CommandClear.hpp"
#include "orpheus/Command/Render/CommandColor.hpp"
#include "orpheus/Command/Render/CommandVertices.hpp"
#include "orpheus/Command/Render/CommandMaterial.hpp"
#include "orpheus/Material/MaterialFlatColor.hpp"

namespace Orpheus::Render {
    class OpenGL : public Render {
    public:
        class Context : public Render::Context {};
        using ContextPtr = std::shared_ptr<Context>;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    private:
        void onCommand(const std::shared_ptr<Command::Render::CommandClear>& command);
        void onCommand(const std::shared_ptr<Command::Render::CommandColor>& command);
        void onCommand(const std::shared_ptr<Command::Render::CommandVertices>& command);
        void onCommand(const std::shared_ptr<Command::Render::CommandMaterial<Orpheus::Material::MaterialFlatColor>>& command);

    public:
        OpenGL(const Window::WindowPtr& window);
        ~OpenGL();

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
