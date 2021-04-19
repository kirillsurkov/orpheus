#pragma once

#include "orpheus/Material/Command/CommandMatrix.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"
#include "orpheus/Math/Vector.hpp"

namespace Orpheus::Material {
    class Text::Command::GlyphModel : public Orpheus::Material::Command::Matrix::Matrix {
    private:
        Math::Vector2 m_textureSize;

    public:
        GlyphModel(float textureWidth, float textureHeight, const Math::Matrix4& matrix) :
            Matrix(matrix),
            m_textureSize(textureWidth, textureHeight)
        {
        }

        virtual std::string getName() const override {
            return "CommandGlyphModel";
        }

        float getTextureWidth() const {
            return m_textureSize.getX();
        }

        float getTextureHeight() const {
            return m_textureSize.getY();
        }
    };
}
