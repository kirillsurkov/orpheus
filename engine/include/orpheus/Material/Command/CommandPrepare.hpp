#pragma once

#include "orpheus/Material/Command/Command.hpp"
#include "orpheus/Vertex.hpp"

namespace Orpheus::Material::Command {
    class Prepare : public Command {
    private:
        const Vertex::Vertices& m_vertices;

    public:
        Prepare(const Vertex::Vertices& vertices) :
            m_vertices(vertices)
        {
        }

        virtual std::string getName() const override {
            return "CommandPrepare";
        }

        const Vertex::Vertices& getVertices() const {
            return m_vertices;
        }
    };
}
