#pragma once

#include <array>
#include <memory>

namespace orpheus::math {
    struct alignas(8) Vector2 {
        float data[2] = {};

        constexpr float& x() {
            return data[0];
        }

        constexpr float& y() {
            return data[1];
        }

        constexpr const float& x() const {
            return data[0];
        }

        constexpr const float& y() const {
            return data[1];
        }
    };

    struct alignas(16) Vector3 {
        float data[4] = {};

        constexpr float& x() {
            return data[0];
        }

        constexpr float& y() {
            return data[1];
        }

        constexpr float& z() {
            return data[2];
        }

        constexpr const float& x() const {
            return data[0];
        }

        constexpr const float& y() const {
            return data[1];
        }

        constexpr const float& z() const {
            return data[2];
        }
    };

    struct alignas(16) Vector4 {
        float data[4] = {};

        constexpr float& x() {
            return data[0];
        }

        constexpr float& y() {
            return data[1];
        }

        constexpr float& z() {
            return data[2];
        }

        constexpr float& w() {
            return data[3];
        }

        constexpr const float& x() const {
            return data[0];
        }

        constexpr const float& y() const {
            return data[1];
        }

        constexpr const float& z() const {
            return data[2];
        }

        constexpr const float& w() const {
            return data[3];
        }
    };

    using Quaternion = Vector4;

    struct alignas(64) Matrix4x4 {
        float data[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
                            {0.0f, 1.0f, 0.0f, 0.0f},
                            {0.0f, 0.0f, 1.0f, 0.0f},
                            {0.0f, 0.0f, 0.0f, 1.0f}};
    };
}

namespace orpheus::interface {
    struct IMath {
        virtual void inverse(math::Matrix4x4& dst, const math::Matrix4x4& mat)                                            = 0;
        virtual void transpose(math::Matrix4x4& dst, const math::Matrix4x4& mat)                                          = 0;

        virtual void mul(math::Matrix4x4& dst, const math::Matrix4x4& lhs, const math::Matrix4x4& rhs)                    = 0;
        virtual void mul(math::Vector4& dst, const math::Matrix4x4& lhs, const math::Vector4& rhs)                        = 0;

        virtual void mul(math::Matrix4x4& mat, float scalar)                                                              = 0;
        virtual void mul(math::Vector2& dst, const math::Vector2& lhs, float rhs)                                         = 0;
        virtual void mul(math::Vector3& dst, const math::Vector3& lhs, float rhs)                                         = 0;
        virtual void mul(math::Vector4& dst, const math::Vector4& lhs, float rhs)                                         = 0;

        virtual void rotate(math::Matrix4x4& dst, float x, float y, float z, float angle)                                 = 0;
        virtual void rotate(math::Vector3& dst, float x, float y, float z, float angle)                                   = 0;

        virtual void translate(math::Matrix4x4& dst, float x, float y, float z)                                           = 0;

        virtual void look(math::Matrix4x4& dst, float eyeX, float eyeY, float eyeZ,
                                                float dirX, float dirY, float dirZ,
                                                float upX,  float upY,  float upZ)                                        = 0;

        virtual void lookAt(math::Matrix4x4& dst, float eyeX, float eyeY, float eyeZ,
                                                  float dstX, float dstY, float dstZ,
                                                  float upX,  float upY,  float upZ)                                      = 0;

        virtual void perspective(math::Matrix4x4& dst, float fov, float aspect, float near, float far)                    = 0;
        virtual void ortho(math::Matrix4x4& dst, float left, float right, float bottom, float top, float near, float far) = 0;
        virtual void scale(math::Matrix4x4& dst, float x, float y, float z)                                               = 0;

        virtual void normalize(math::Vector3& vec)                                                                        = 0;

        virtual void add(math::Vector2& dst, const math::Vector2& lhs, const math::Vector2& rhs)                          = 0;
        virtual void add(math::Vector3& dst, const math::Vector3& lhs, const math::Vector3& rhs)                          = 0;
        virtual void add(math::Vector4& dst, const math::Vector4& lhs, const math::Vector4& rhs)                          = 0;

        virtual void sub(math::Vector2& dst, const math::Vector2& lhs, const math::Vector2& rhs)                          = 0;
        virtual void sub(math::Vector3& dst, const math::Vector3& lhs, const math::Vector3& rhs)                          = 0;
        virtual void sub(math::Vector4& dst, const math::Vector4& lhs, const math::Vector4& rhs)                          = 0;
    };
}
