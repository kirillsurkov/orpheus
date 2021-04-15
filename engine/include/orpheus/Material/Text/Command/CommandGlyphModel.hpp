#pragma once

#include "orpheus/Material/Command/CommandMatrix.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"

namespace Orpheus::Material {
    class Text::Command::GlyphModel : public Orpheus::Material::Command::Matrix::Matrix {
    public:
        GlyphModel(const glm::mat4& matrix) : Matrix(matrix) {
        }

        virtual std::string getName() const override {
            return "CommandGlyphModel";
        }
    };
}
