#pragma once

#include "orpheus/Utils.hpp"
#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Command/Render/RenderCommand.hpp"

namespace Orpheus::Entity {
    template<class T>
    class EntityCommand : public Entity {
    private:
        Command::Render::RenderCommandPtr m_command;
        std::function<void(Render::Render&)> m_draw;

    public:
        template<class... Args>
        EntityCommand(Args&&... args) :
            m_command(createRenderCommand<T>(std::forward<Args>(args)...)),
            m_draw([this](Render::Render& render) {
                render.postRenderCommand(std::static_pointer_cast<T>(m_command));
            })
        {
        }

        virtual void draw(Render::Render& render) override {
            m_draw(render);
        }
    };
}
