#pragma once

#include <array>
#include <memory>

#include "orpheus/Math/Vector.hpp"

namespace Orpheus::Math {
    class Matrix4 {
    private:
        float m_data[16];

        Matrix4(const float* data);

    public:
        Matrix4();

        const float* getData() const;

        Matrix4 mul(const Matrix4& mat) const;
        Vector4 mul(const Vector4& vec) const;

        Matrix4 translate(float x, float y, float z) const;
        Matrix4 scale(float x, float y, float z) const;
        Matrix4 rotate(float angle, float x, float y, float z) const;

        Matrix4 inverse() const;

        static Matrix4 perspective(float fov, float aspect, float near, float far);
        static Matrix4 orthogonal(float left, float right, float bottom, float top);
    };
}
