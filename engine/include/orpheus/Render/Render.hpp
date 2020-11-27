#pragma once

#include "orpheus/Log.hpp"

namespace Orpheus::Render {
    class Render : public Loggable {
    public:
        class Context {
        public:
            virtual ~Context() = default;
        };

        using ContextPtr = std::shared_ptr<Context>;

        Render() {
            addScope("Render");
        }
    };

    using RenderPtr = std::shared_ptr<Render>;
}
