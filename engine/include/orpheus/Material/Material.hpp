#pragma once

#include <memory>

namespace Orpheus::Material {
    class Material {
    public:
        virtual ~Material() {}

        virtual std::string getName() const = 0;
    };

    using MaterialPtr = std::shared_ptr<Material>;
}
