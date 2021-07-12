#pragma once

#include "orpheus/interface/IMath.hpp"

namespace orpheus {
    class Camera {
    private:
        std::shared_ptr<interface::IMath> m_math;
        math::Matrix4x4                   m_transform;
        orpheus::math::Vector3            m_position;
        orpheus::math::Vector2            m_rotation;
        orpheus::math::Vector3            m_forward;
        orpheus::math::Vector3            m_right;
        bool                              m_flyForward;
        bool                              m_flyBackward;
        bool                              m_flyLeft;
        bool                              m_flyRight;

    public:
        Camera(const std::shared_ptr<interface::IMath>& math);

        void flyForward(bool enable);
        void flyBackward(bool enable);
        void flyLeft(bool enable);
        void flyRight(bool enable);

        void setPosition(const math::Vector3& position);

        void update(const math::Vector2& rotation, float delta);

        const math::Vector3& getForward() const;
        const math::Vector3& getRight() const;

        const math::Matrix4x4& getTransform() const;
    };
}
