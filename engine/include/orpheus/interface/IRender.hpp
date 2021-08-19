#pragma once

#include "orpheus/interface/IMath.hpp"

#include <memory>
#include <vector>

namespace orpheus::render::material {
    struct FlatColor {
        math::Vector3 color;
    };

    struct GGX {
        float              roughness;
        std::vector<float> lightPoints;
        std::vector<float> lightColors;
        std::vector<int>   lightIndices;
    };
}

namespace orpheus::interface {
    struct IRender {
        virtual void init()                                    = 0;
        virtual void startFrame()                              = 0;
        virtual void endFrame()                                = 0;
        virtual void setProjection(const math::Matrix4x4& mat) = 0;
        virtual void setView(const math::Matrix4x4& mat)       = 0;
        virtual void setModel(const math::Matrix4x4& mat)      = 0;
        virtual void clear(float r, float g, float b, float a) = 0;
        virtual void drawCube()                                = 0;
        virtual void drawSphere()                              = 0;
        virtual void drawCylinder()                            = 0;
        virtual void drawPlane()                               = 0;
        virtual void drawBumpy()                               = 0;

        virtual void setMaterial(const render::material::FlatColor& material) = 0;
        virtual void setMaterial(const render::material::GGX&       material) = 0;
    };
}