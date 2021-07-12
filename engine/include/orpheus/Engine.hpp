#pragma once

#include "orpheus/Scene.hpp"
#include "orpheus/async/Executor.hpp"
#include "orpheus/interface/IInput.hpp"
#include "orpheus/interface/IMath.hpp"
#include "orpheus/interface/IPhysics.hpp"
#include "orpheus/interface/IRender.hpp"
#include "orpheus/interface/IRenderContext.hpp"
#include "orpheus/interface/IWindow.hpp"

namespace orpheus {
    class Engine
    {
    private:
        bool                                       m_running;
        float                                      m_fpsLimit;

        std::shared_ptr<interface::IWindow>        m_window;
        std::shared_ptr<interface::IRenderContext> m_renderContext;

        std::shared_ptr<interface::IRender>        m_render;
        async::Executor                            m_renderExecutor;

        std::shared_ptr<interface::IInput>         m_input;
        async::Executor                            m_inputExecutor;

        std::shared_ptr<interface::IPhysics>       m_physics;
        async::Executor                            m_physicsExecutor;

        std::shared_ptr<interface::IMath>          m_math;

        std::shared_ptr<Scene>                     m_scene;

    public:
        Engine(const std::shared_ptr<interface::IWindow>&  window,
               const std::shared_ptr<interface::IRender>&  render,
               const std::shared_ptr<interface::IInput>&   input,
               const std::shared_ptr<interface::IPhysics>& physics,
               const std::shared_ptr<interface::IMath>&    math,
               const std::shared_ptr<Scene>&               scene);

        ~Engine();

        void run();
    };
}
