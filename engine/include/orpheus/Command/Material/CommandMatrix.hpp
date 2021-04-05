#pragma once

#include "orpheus/Command/Material/Command.hpp"

#include <glm/mat4x4.hpp>

namespace Orpheus::Command::Material {
    class CommandMatrix : public Command {
    private:
        glm::mat4x4 m_matrix;

    public:
        CommandMatrix(const glm::mat4x4& matrix) :
            m_matrix(matrix)
        {
        }

        const glm::mat4x4& getMatrix() const {
            return m_matrix;
        }
    };
}
