#pragma once

#include "orpheus/Log.hpp"

namespace Orpheus::Scene {
    class Scene : public Loggable {
    public:
        Scene() {
            addScope("Scene");
        }

        virtual ~Scene() {
        }
    };
}
