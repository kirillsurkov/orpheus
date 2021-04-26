#pragma once

#include "orpheus/Dispatcher.hpp"
#include "orpheus/Render/impl/OpenGL/Shader/Program.hpp"

namespace Orpheus::Render::OpenGLImpl::Material {
    class Material {
    private:
        CommandDispatcher m_commandDispatcher;

    protected:
        template<class T, class U>
        void registerMaterialCommand(U&& receiver) {
            m_commandDispatcher.registerCommand<T>(std::forward<U>(receiver));
        }

    public:
        template<class T>
        void postCommand(T&& command) {
            m_commandDispatcher.dispatchOrThrow(std::forward<T>(command));
        }
    };

    using MaterialPtr = std::shared_ptr<Material>;
}
