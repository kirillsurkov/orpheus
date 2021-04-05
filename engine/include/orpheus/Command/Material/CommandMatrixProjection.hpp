#pragma once

#include "orpheus/Command/Material/CommandMatrix.hpp"

namespace Orpheus::Command::Material {
    class CommandMatrixProjection : public CommandMatrix {
    public:
        CommandMatrixProjection(const glm::mat4x4& matrix) : CommandMatrix(matrix) {
        }

        virtual std::string getName() const override {
            return "CommandMatrixProjection";
        }
    };
}
