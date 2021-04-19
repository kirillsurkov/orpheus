#pragma once

#include "orpheus/Material/Command/CommandMatrix.hpp"

namespace Orpheus::Material::Command {
    class MatrixView : public Matrix {
    public:
        MatrixView(const Math::Matrix4& matrix) : Matrix(matrix) {
        }

        virtual std::string getName() const override {
            return "CommandMatrixView";
        }
    };
}
