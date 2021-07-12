#include "math/cglm/CGLM.hpp"

#include <cglm/cglm.h>
#include <cglm/call.h>

namespace orpheus::math::cglm {
    void CGLM::inverse(Matrix4x4& dst, const Matrix4x4& mat) {
        glm_mat4_inv(const_cast<Matrix4x4&>(mat).data, dst.data);
    }

    void CGLM::transpose(Matrix4x4& dst, const Matrix4x4& mat) {
        glm_mat4_transpose_to(const_cast<Matrix4x4&>(mat).data, dst.data);
    }

    void CGLM::mul(Matrix4x4& dst, const Matrix4x4& lhs, const Matrix4x4& rhs) {
        glm_mat4_mul(const_cast<Matrix4x4&>(lhs).data, const_cast<Matrix4x4&>(rhs).data, dst.data);
    }

    void CGLM::mul(Vector4& dst, const Matrix4x4& lhs, const Vector4& rhs) {
        glm_mat4_mulv(const_cast<Matrix4x4&>(lhs).data, const_cast<Vector4&>(rhs).data, dst.data);
    }

    void CGLM::mul(Matrix4x4& mat, float scalar) {
        glm_mat4_scale(mat.data, scalar);
    }

    void CGLM::mul(Vector4& dst, const Vector4& lhs, float rhs) {
        glm_vec4_scale(const_cast<Vector4&>(lhs).data, rhs, dst.data);
    }

    void CGLM::mul(Vector3& dst, const Vector3& lhs, float rhs) {
        glm_vec3_scale(const_cast<Vector3&>(lhs).data, rhs, dst.data);
    }

    void CGLM::mul(Vector2& dst, const Vector2& lhs, float rhs) {
        glm_vec2_scale(const_cast<Vector2&>(lhs).data, rhs, dst.data);
    }

    void CGLM::rotate(Matrix4x4& dst, float x, float y, float z, float angle) {
        Vector3 axis{{x, y, z}};
        glm_rotate(dst.data, angle, axis.data);
    }

    void CGLM::rotate(Vector3& dst, float x, float y, float z, float angle) {
        Vector3 axis{{x, y, z}};
        glm_vec3_rotate(dst.data, angle, axis.data);
    }

    void CGLM::translate(Matrix4x4& dst, float x, float y, float z) {
        Vector3 vec{{x, y, z}};
        glm_translate(dst.data, vec.data);
    }

    void CGLM::look(Matrix4x4& dst, float eyeX, float eyeY, float eyeZ,
                                    float dirX, float dirY, float dirZ,
                                    float upX,  float upY,  float upZ) {
        Vector3 eye{eyeX, eyeY, eyeZ};
        Vector3 dir{dirX, dirY, dirZ};
        Vector3 up{upX, upY, upZ};
        glm_look(eye.data, dir.data, up.data, dst.data);
    }

    void CGLM::lookAt(Matrix4x4& dst, float eyeX, float eyeY, float eyeZ,
                                      float dstX, float dstY, float dstZ,
                                      float upX,  float upY,  float upZ) {
        Vector3 eye{eyeX, eyeY, eyeZ};
        Vector3 center{dstX, dstY, dstZ};
        Vector3 up{upX, upY, upZ};
        glm_lookat(eye.data, center.data, up.data, dst.data);
    }

    void CGLM::perspective(Matrix4x4& dst, float fov, float aspect, float near, float far) {
        glm_perspective(fov, aspect, near, far, dst.data);
    }

    void CGLM::ortho(Matrix4x4& dst, float left, float right, float bottom, float top, float near, float far) {
        glm_ortho(left, right, bottom, top, near, far, dst.data);
    }

    void CGLM::scale(Matrix4x4& dst, float x, float y, float z) {
        Vector3 vec{x, y, z};
        glm_scale(dst.data, vec.data);
    }

    void CGLM::normalize(Vector3& vec) {
        glm_normalize(vec.data);
    }

    void CGLM::add(Vector2& dst, const Vector2& lhs, const Vector2& rhs) {
        glm_vec2_add(const_cast<Vector2&>(lhs).data, const_cast<Vector2&>(rhs).data, dst.data);
    }

    void CGLM::add(Vector3& dst, const Vector3& lhs, const Vector3& rhs) {
        glm_vec3_add(const_cast<Vector3&>(lhs).data, const_cast<Vector3&>(rhs).data, dst.data);
    }

    void CGLM::add(Vector4& dst, const Vector4& lhs, const Vector4& rhs) {
        glm_vec4_add(const_cast<Vector4&>(lhs).data, const_cast<Vector4&>(rhs).data, dst.data);
    }

    void CGLM::sub(Vector2& dst, const Vector2& lhs, const Vector2& rhs) {
        glm_vec2_sub(const_cast<Vector2&>(lhs).data, const_cast<Vector2&>(rhs).data, dst.data);
    }

    void CGLM::sub(Vector3& dst, const Vector3& lhs, const Vector3& rhs) {
        glm_vec3_sub(const_cast<Vector3&>(lhs).data, const_cast<Vector3&>(rhs).data, dst.data);
    }

    void CGLM::sub(Vector4& dst, const Vector4& lhs, const Vector4& rhs) {
        glm_vec4_sub(const_cast<Vector4&>(lhs).data, const_cast<Vector4&>(rhs).data, dst.data);
    }
}
