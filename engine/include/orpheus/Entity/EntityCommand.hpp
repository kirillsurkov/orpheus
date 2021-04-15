#pragma once

#include "orpheus/Utils.hpp"
#include "orpheus/Entity/Entity.hpp"

namespace Orpheus::Entity {
    template<class T>
    class Command : public Entity {
    private:
        T m_command;
        std::function<void(Render::Render&)> m_draw;

    public:
        template<class... Args>
        Command(Vertex::BufferCache&, Args&&... args) :
            m_command(std::forward<Args>(args)...),
            m_draw([this](Render::Render& render) {
                render.postCommand(m_command);
            })
        {
        }

        virtual void draw(const glm::mat4x4&, const glm::mat4x4&, Render::Render& render) const override {
            m_draw(render);
        }

        T& getCommand() {
            return m_command;
        }
    };
}
