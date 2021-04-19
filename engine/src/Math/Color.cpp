#include "orpheus/Math/Color.hpp"
#include <glm/gtx/color_space.hpp>

Orpheus::Math::Color Orpheus::Math::Color::fromHSV(float h, float s, float v, float a) {
    auto color = glm::rgbColor(glm::vec3(h, s, v));
    return Color(color.r, color.g, color.b, a);
}

Orpheus::Math::Color::Color(float r, float g, float b, float a) :
    m_r(r), m_g(g), m_b(b), m_a(a)
{
}

float Orpheus::Math::Color::getR() const {
    return m_r;
}

float Orpheus::Math::Color::getG() const {
    return m_g;
}

float Orpheus::Math::Color::getB() const {
    return m_b;
}

float Orpheus::Math::Color::getA() const {
    return m_a;
}

Orpheus::Math::Color& Orpheus::Math::Color::set(float r, float g, float b, float a) {
    m_r = r;
    m_g = g;
    m_b = b;
    m_a = a;
    return *this;
}

Orpheus::Math::Color& Orpheus::Math::Color::set(const Color& color) {
    return set(color.m_r, color.m_g, color.m_b, color.m_a);
}

void Orpheus::Math::Color::setR(float r) {
    m_r = r;
}

void Orpheus::Math::Color::setG(float g) {
    m_g = g;
}

void Orpheus::Math::Color::setB(float b) {
    m_b = b;
}

void Orpheus::Math::Color::setA(float a) {
    m_a = a;
}
