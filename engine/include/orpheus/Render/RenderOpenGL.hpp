#pragma once

#include "orpheus/Render/Render.hpp"
#include "orpheus/Window/Window.hpp"
#include "orpheus/Utils.hpp"
#include "orpheus/Command/Render/CommandClear.hpp"
#include "orpheus/Command/Render/CommandSetClearColor.hpp"
#include "orpheus/Command/Render/CommandVertices.hpp"

namespace Orpheus::Render {
    class OpenGL : public Render {
    public:
        class Context : public Render::Context {};
        using ContextPtr = std::shared_ptr<Context>;

    private:
        struct Vertices {
            unsigned int vao;
            unsigned int count;
        };

        std::unordered_map<std::shared_ptr<Command::Render::CommandVertices::AttribArray>, unsigned int> m_attribs;
        std::unordered_map<std::shared_ptr<Command::Render::CommandVertices>, Vertices> m_vertices;

    private:
        void onCommand(const std::shared_ptr<Command::Render::CommandClear>& command);
        void onCommand(const std::shared_ptr<Command::Render::CommandSetClearColor>& command);
        void onCommand(const std::shared_ptr<Command::Render::CommandVertices>& command);

    protected:
        virtual void onCreate() override;

    public:
        OpenGL();
        ~OpenGL();

        template<class T>
        void postCommand(T&& command) {
            onCommand(command);
        }
    };
}
