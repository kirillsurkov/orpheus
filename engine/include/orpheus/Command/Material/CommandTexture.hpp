#pragma once

#include "orpheus/Command/Material/Command.hpp"

namespace Orpheus::Command::Material {
    class CommandTexture : public Command {
    private:
        unsigned int m_textureId;

    public:
        CommandTexture(unsigned int textureId) :
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
