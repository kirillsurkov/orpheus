#pragma once

#include "orpheus/Material/Command/CommandMatrix.hpp"

namespace Orpheus::Material::Command {
    class MatrixProjection : public Matrix {
    public:
        MatrixProjection(const glm::mat4x4& matrix) : Matrix(matrix) {
        }

        virtual std::string getName() const override {
            return "CommandMatrixProjection";
        }
    };
}
