#pragma once

#include "orpheus/Material/Command/Command.hpp"

namespace Orpheus::Material::Command {
    class Texture : public Command {
    private:
        unsigned int m_textureId;

    public:
        Texture(unsigned int textureId) :
            m_textureId(textureId)
        {
        }

        virtual std::string getName() const override {
            return "CommandTexture";
        }

        unsigned int getTextureId() const {
            return m_textureId;
        }
    };
}
