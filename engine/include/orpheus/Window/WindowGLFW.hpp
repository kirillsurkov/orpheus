#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/RenderOpenGL.hpp"

namespace Orpheus::Window {
    class GLFW : public Window {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        GLFW(const std::string& title, unsigned int width, unsigned int height);
        virtual ~GLFW();

        virtual void swapBuffers() override;

        void createContext(Render::OpenGL::ContextPtr& context);
    };
};
