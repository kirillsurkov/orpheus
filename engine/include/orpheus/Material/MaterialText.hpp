#pragma once

#include "orpheus/Material/Material.hpp"

namespace Orpheus::Material {
    class MaterialText : public Material {
    public:
        virtual std::string getName() const override {
            return "MaterialText";
        }
    };
}
