#pragma once

#include "orpheus/Material/Command/CommandMatrix.hpp"

namespace Orpheus::Material::Command {
    class MatrixModel : public Matrix {
    public:
        MatrixModel(const glm::mat4x4& matrix) : Matrix(matrix) {
        }

        virtual std::string getName() const override {
            return "CommandMatrixModel";
        }
    };
}
