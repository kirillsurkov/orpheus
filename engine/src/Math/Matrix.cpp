#include "orpheus/Math/Matrix.hpp"
#include <glm/ext.hpp>

Orpheus::Math::Matrix4::Matrix4(const float* data) {
    std::copy(data, data + 16, m_data);
}

Orpheus::Math::Matrix4::Matrix4() :
    Matrix4(glm::value_ptr(glm::mat4(1.0f)))
{
}

const float* Orpheus::Math::Matrix4::getData() const {
    return m_data;
}

Orpheus::Math::Matrix4 Orpheus::Math::Matrix4::mul(const Orpheus::Math::Matrix4& mat) const {
    return Matrix4(glm::value_ptr(glm::make_mat4(m_data) * glm::make_mat4(mat.m_data)));
}

Orpheus::Math::Vector4 Orpheus::Math::Matrix4::mul(const Orpheus::Math::Vector4& vec) const {
    auto res = glm::make_mat4(m_data) * glm::vec4(vec.getX(), vec.getY(), vec.getZ(), vec.getW());
    return Vector4(res.x, res.y, res.z, res.w);
}

Orpheus::Math::Matrix4 Orpheus::Math::Matrix4::translate(float x, float y, float z) const {
    return Matrix4(glm::value_ptr(glm::translate(glm::make_mat4(m_data), glm::vec3(x, y, z))));
}

Orpheus::Math::Matrix4 Orpheus::Math::Matrix4::scale(float x, float y, float z) const {
    return Matrix4(glm::value_ptr(glm::scale(glm::make_mat4(m_data), glm::vec3(x, y, z))));
}

Orpheus::Math::Matrix4 Orpheus::Math::Matrix4::inverse() const {
    return Matrix4(glm::value_ptr(glm::inverse(glm::make_mat4(m_data))));
}

Orpheus::Math::Matrix4 Orpheus::Math::Matrix4::perspective(float fov, float aspect, float near, float far) {
    return Matrix4(glm::value_ptr(glm::perspective(fov, aspect, near, far)));
}

Orpheus::Math::Matrix4 Orpheus::Math::Matrix4::orthogonal(float left, float right, float bottom, float top) {
    return Matrix4(glm::value_ptr(glm::ortho(left, right, bottom, top)));
}
