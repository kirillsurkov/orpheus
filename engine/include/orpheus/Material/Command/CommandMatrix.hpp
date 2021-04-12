#pragma once

#include "orpheus/Material/Command/Command.hpp"

#include <glm/mat4x4.hpp>

namespace Orpheus::Material::Command {
    class Matrix : public Command {
    private:
        glm::mat4x4 m_matrix;

    public:
        Matrix(const glm::mat4x4& matrix) :
            m_matrix(matrix)
        {
        }

        const glm::mat4x4& getMatrix() const {
            return m_matrix;
        }
    };
}
