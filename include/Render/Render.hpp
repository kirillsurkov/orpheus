#pragma once

#include <memory>

class Render {
public:
    class Context {
    public:
        virtual ~Context() = default;
    };

    using ContextPtr = std::shared_ptr<Context>;
};

using RenderPtr = std::shared_ptr<Render>;
