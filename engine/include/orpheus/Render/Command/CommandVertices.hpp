#pragma once

#include "orpheus/Render/Command/Command.hpp"
#include "orpheus/Vertex.hpp"

#include <vector>

namespace Orpheus::Render::Command {
    class Vertices : public Command {
    private:
        const Vertex::Vertices& m_vertices;

    public:
        Vertices(const Vertex::Vertices& vertices) :
            m_vertices(vertices)
        {
        }

        virtual std::string getName() const override {
            return "CommandVertices";
        }

        const Vertex::Vertices& getVertices() const {
            return m_vertices;
        }
    };
}
