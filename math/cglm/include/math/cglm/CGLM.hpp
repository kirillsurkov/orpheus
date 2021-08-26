#pragma once

#include <orpheus/interface/IMath.hpp>

namespace orpheus::math::cglm {
    class CGLM : public interface::IMath {
    public:
        virtual void inverse(Matrix4x4& dst, const Matrix4x4& mat)                                                  override;
        virtual void transpose(Matrix4x4& dst, const Matrix4x4& mat)                                                override;

        virtual void mul(Matrix4x4& dst, const Matrix4x4& lhs, const Matrix4x4& rhs)                                override;
        virtual void mul(Vector4& dst, const Matrix4x4& lhs, const Vector4& rhs)                                    override;

        virtual void mul(Matrix4x4& mat, float scalar)                                                              override;
        virtual void mul(Vector2& dst, const Vector2& lhs, float rhs)                                               override;
        virtual void mul(Vector3& dst, const Vector3& lhs, float rhs)                                               override;
        virtual void mul(Vector4& dst, const Vector4& lhs, float rhs)                                               override;

        virtual void rotate(Matrix4x4& dst, float x, float y, float z, float angle)                                 override;
        virtual void rotate(Vector3& dst,   float x, float y, float z, float angle)                                 override;

        virtual void translate(Matrix4x4& dst, float x, float y, float z)                                           override;

        virtual void look(Matrix4x4& dst, float eyeX, float eyeY, float eyeZ,
                                          float dirX, float dirY, float dirZ,
                                          float upX,  float upY,  float upZ)                                        override;
        virtual void lookAt(Matrix4x4& dst, float eyeX, float eyeY, float eyeZ,
                                          float dstX, float dstY, float dstZ,
                                          float upX,  float upY,  float upZ)                                        override;

        virtual void perspective(Matrix4x4& dst, float fov, float aspect, float near, float far)                    override;
        virtual void ortho(Matrix4x4& dst, float left, float right, float bottom, float top, float near, float far) override;
        virtual void scale(Matrix4x4& dst, float x, float y, float z)                                               override;

        virtual void normalize(Vector3& vec)                                                                        override;

        virtual void add(Vector2& dst, const Vector2& lhs, const Vector2& rhs)                                      override;
        virtual void add(Vector3& dst, const Vector3& lhs, const Vector3& rhs)                                      override;
        virtual void add(Vector4& dst, const Vector4& lhs, const Vector4& rhs)                                      override;

        virtual void sub(Vector2& dst, const Vector2& lhs, const Vector2& rhs)                                      override;
        virtual void sub(Vector3& dst, const Vector3& lhs, const Vector3& rhs)                                      override;
        virtual void sub(Vector4& dst, const Vector4& lhs, const Vector4& rhs)                                      override;

        virtual void hsv2rgb(Vector3& dst, float h, float s, float v)                                               override;
    };
}
