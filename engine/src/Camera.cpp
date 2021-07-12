#include "orpheus/Camera.hpp"

#include <cmath>

namespace orpheus {
    Camera::Camera(const std::shared_ptr<interface::IMath>& math) :
        m_math(math)
    { }

    void Camera::flyForward(bool enable) {
        m_flyForward = enable;
    }

    void Camera::flyBackward(bool enable) {
        m_flyBackward = enable;
    }

    void Camera::flyLeft(bool enable) {
        m_flyLeft = enable;
    }

    void Camera::flyRight(bool enable) {
        m_flyRight = enable;
    }

    void Camera::setPosition(const math::Vector3& position) {
        m_position = position;
    }

    void Camera::update(const math::Vector2& rotation, float delta) {
        m_rotation.x() += rotation.y();
        m_rotation.y() -= rotation.x();

        m_forward = { std::cos(m_rotation.x()) * std::sin(m_rotation.y()),
                      std::sin(m_rotation.x()),
                     -std::cos(m_rotation.x()) * std::cos(m_rotation.y())};
        m_right = {-m_forward.z(), 0.0f, m_forward.x()};

        orpheus::math::Vector3 translation;
        if (m_flyForward)  m_math->add(translation, translation, m_forward);
        if (m_flyBackward) m_math->sub(translation, translation, m_forward);
        if (m_flyLeft)     m_math->sub(translation, translation, m_right);
        if (m_flyRight)    m_math->add(translation, translation, m_right);
        if (translation.x() || translation.y() || translation.z()) {
            m_math->normalize(translation);
            m_math->mul(translation, translation, delta * 10);
            m_math->add(m_position, m_position, translation);
        }

        m_math->look(m_transform,
                     m_position.x(), m_position.y(), m_position.z(),
                     m_forward.x(),  m_forward.y(),  m_forward.z(),
                     0.0f, 1.0f, 0.0f);
    }

    const math::Vector3& Camera::getForward() const {
        return m_forward;
    }

    const math::Vector3& Camera::getRight() const {
        return m_right;
    }

    const math::Matrix4x4& Camera::getTransform() const {
        return m_transform;
    }
}
