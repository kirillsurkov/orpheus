#pragma once

#include "orpheus/interface/IMath.hpp"

#include <memory>

namespace orpheus::interface {
    struct IRender {
        virtual void init()                                    = 0;
        virtual void setProjection(const math::Matrix4x4& mat) = 0;
        virtual void setView(const math::Matrix4x4& mat)       = 0;
        virtual void setModel(const math::Matrix4x4& mat)      = 0;
        virtual void clear(float r, float g, float b, float a) = 0;
        virtual void drawCube()                                = 0;
        virtual void drawSphere()                              = 0;
    };
}
