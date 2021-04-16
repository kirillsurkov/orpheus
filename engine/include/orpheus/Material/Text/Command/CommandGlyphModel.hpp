#pragma once

#include "orpheus/Material/Command/CommandMatrix.hpp"
#include "orpheus/Material/Text/MaterialText.hpp"

namespace Orpheus::Material {
    class Text::Command::GlyphModel : public Orpheus::Material::Command::Matrix::Matrix {
    private:
        float m_textureWidth;
        float m_textureHeight;

    public:
        GlyphModel(float textureWidth, float textureHeight, const glm::mat4& matrix) :
            Matrix(matrix),
            m_textureWidth(textureWidth),
            m_textureHeight(textureHeight)
        {
        }

        virtual std::string getName() const override {
            return "CommandGlyphModel";
        }

        float getTextureWidth() const {
            return m_textureWidth;
        }

        float getTextureHeight() const {
            return m_textureHeight;
        }
    };
}
