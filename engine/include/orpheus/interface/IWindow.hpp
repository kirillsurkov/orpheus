#pragma once

#include "orpheus/interface/IRenderContext.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace orpheus::interface {
    struct IWindow {
        virtual void                            create(const std::string& title)       = 0;
        virtual std::shared_ptr<IRenderContext> createContext()                        = 0;
        virtual void                            destroy()                              = 0;
        virtual void                            swapBuffers()                          = 0;
        virtual std::uint32_t                   getWidth()                       const = 0;
        virtual std::uint32_t                   getHeight()                      const = 0;
    };
}
