#pragma once

#include "orpheus/Material/Material.hpp"

namespace Orpheus::Material {
    class FlatColor : public Material {
    public:
        virtual std::string getName() const override {
            return "MaterialFlatColor";
        }
    };
}
