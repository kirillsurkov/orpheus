#include "math/cglm/CGLM.hpp"

#include <cglm/cglm.h>
#include <cglm/call.h>

namespace orpheus::math::cglm {
    void CGLM::mul(interface::math::Matrix4x4& dst, const interface::math::Matrix4x4& rhs) {
        glm_mat4_mul(dst.data, const_cast<interface::math::Matrix4x4&>(rhs).data, dst.data);
    }

    void CGLM::perspective(interface::math::Matrix4x4& dst, float fov, float aspect, float near, float far) {
        glm_perspective(fov, aspect, near, far, dst.data);
    }

    void CGLM::ortho(interface::math::Matrix4x4& dst, float left, float right, float bottom, float top, float near, float far) {
        glm_ortho(left, right, bottom, top, near, far, dst.data);
    }

    void CGLM::translate(interface::math::Matrix4x4& dst, float x, float y, float z) {
        interface::math::Vector3 vec{x, y, z};
        glm_translate(dst.data, &vec.x);
    }

    void CGLM::rotate(interface::math::Matrix4x4& dst, float x, float y, float z, float angle) {
        interface::math::Vector3 axis{x, y, z};
        glm_rotate(dst.data, angle, &axis.x);
    }

    void CGLM::scale(interface::math::Matrix4x4& dst, float x, float y, float z) {
        interface::math::Vector3 vec{x, y, z};
        glm_scale(dst.data, &vec.x);
    }
}
