#pragma once

#include "orpheus/Material/Command/Command.hpp"
#include "orpheus/Math/Matrix.hpp"

namespace Orpheus::Material::Command {
    class Matrix : public Command {
    private:
        Math::Matrix4 m_matrix;

    public:
        Matrix(const Math::Matrix4& matrix) :
            m_matrix(matrix)
        {
        }

        const Math::Matrix4& getMatrix() const {
            return m_matrix;
        }
    };
}
