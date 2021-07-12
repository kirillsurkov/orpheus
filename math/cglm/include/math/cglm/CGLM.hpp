#pragma once

#include <orpheus/interface/IMath.hpp>

namespace orpheus::math::cglm {
    class CGLM : public interface::IMath {
    public:
        virtual void mul(interface::math::Matrix4x4& dst, const interface::math::Matrix4x4& rhs)                                     override;
        virtual void perspective(interface::math::Matrix4x4& dst, float fov, float aspect, float near, float far)                    override;
        virtual void ortho(interface::math::Matrix4x4& dst, float left, float right, float bottom, float top, float near, float far) override;
        virtual void translate(interface::math::Matrix4x4& dst, float x, float y, float z)                                           override;
        virtual void rotate(interface::math::Matrix4x4& dst, float x, float y, float z, float angle)                                 override;
        virtual void scale(interface::math::Matrix4x4& dst, float x, float y, float z)                                               override;
    };
}
