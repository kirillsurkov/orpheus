#pragma once

#include "orpheus/Material/Command/Command.hpp"

namespace Orpheus::Material::Command {
    class Texture : public Command {
    private:
        std::size_t m_index;

    public:
        Texture(const std::size_t& index) :
            m_index(index)
        {
        }

        virtual std::string getName() const override {
            return "CommandTexture";
        }

        const std::size_t& getIndex() const {
            return m_index;
        }
    };
}
