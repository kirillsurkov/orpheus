#pragma once

#include "orpheus/Material/Material.hpp"

namespace Orpheus::Material {
    class MaterialFlatColor : public Material {
    public:
        virtual std::string getName() const override {
            return "MaterialFlatColor";
        }
    };
}
