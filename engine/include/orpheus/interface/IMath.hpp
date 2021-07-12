#pragma once

#include <array>
#include <memory>

namespace orpheus::interface {
    namespace math {
        struct alignas(16) Vector3 {
            float x    = 0.0f;
            float y    = 0.0f;
            float z    = 0.0f;
            float _pad = 0.0f;
        };

        struct alignas(16) Vector4 {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
            float w = 0.0f;
        };

        struct alignas(64) Matrix4x4 {
            float data[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
                                {0.0f, 1.0f, 0.0f, 0.0f},
                                {0.0f, 0.0f, 1.0f, 0.0f},
                                {0.0f, 0.0f, 0.0f, 1.0f}};
        };
    }

    struct IMath {
        virtual void mul(interface::math::Matrix4x4& dst, const interface::math::Matrix4x4& rhs)                                     = 0;
        virtual void perspective(interface::math::Matrix4x4& dst, float fov, float aspect, float near, float far)                    = 0;
        virtual void ortho(interface::math::Matrix4x4& dst, float left, float right, float bottom, float top, float near, float far) = 0;
        virtual void translate(interface::math::Matrix4x4& dst, float x, float y, float z)                                           = 0;
        virtual void rotate(interface::math::Matrix4x4& dst, float x, float y, float z, float angle)                                 = 0;
        virtual void scale(interface::math::Matrix4x4& dst, float x, float y, float z)                                               = 0;
    };
}
