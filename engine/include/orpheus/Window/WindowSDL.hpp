#pragma once

#include "orpheus/Window/Window.hpp"
#include "orpheus/Render/RenderOpenGL.hpp"

namespace Orpheus::Window {
    class SDL : public Window {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        SDL(const std::string& title, unsigned int width, unsigned int height);
        virtual ~SDL();

        void createContext(Render::OpenGL::ContextPtr& context);
        virtual void swapBuffers() override;
    };
};
