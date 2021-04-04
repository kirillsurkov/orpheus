#pragma once

#include "orpheus/Utils.hpp"
#include "orpheus/Entity/Entity.hpp"

namespace Orpheus::Entity {
    template<class T>
    class EntityCommand : public Entity {
    private:
        T m_command;
        std::function<void(Render::Render&)> m_draw;

    public:
        template<class... Args>
        EntityCommand(Args&&... args) :
            m_command(std::forward<Args>(args)...),
            m_draw([this](Render::Render& render) {
                render.postRenderCommand(m_command);
            })
        {
        }

        virtual void draw(Render::Render& render) override {
            m_draw(render);
        }

        T& getCommand() {
            return m_command;
        }
    };
}
