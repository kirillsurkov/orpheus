#pragma once

namespace Orpheus::Math {
    class Vector2 {
    private:
        float m_x;
        float m_y;

    public:
        Vector2(float x = 0.0f, float y = 0.0f) :
            m_x(x), m_y(y)
        {
        }

        float getX() const {
            return m_x;
        }

        float getY() const {
            return m_y;
        }

        Vector2& set(float x, float y) {
            m_x = x;
            m_y = y;
            return *this;
        }

        Vector2& add(float x, float y) {
            m_x += x;
            m_y += y;
            return *this;
        }

        Vector2& mul(float x, float y) {
            m_x *= x;
            m_y *= y;
            return *this;
        }

        void setX(float x) {
            m_x = x;
        }

        void setY(float y) {
            m_y = y;
        }
    };

    class Vector3 {
    private:
        float m_x;
        float m_y;
        float m_z;

    public:
        Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) :
            m_x(x), m_y(y), m_z(z)
        {
        }

        Vector2 getXY() const {
            return Vector2(m_x, m_y);
        }

        float getX() const {
            return m_x;
        }

        float getY() const {
            return m_y;
        }

        float getZ() const {
            return m_z;
        }

        Vector3& set(float x, float y, float z) {
            m_x = x;
            m_y = y;
            m_z = z;
            return *this;
        }

        Vector3& add(float x, float y, float z) {
            m_x += x;
            m_y += y;
            m_z += z;
            return *this;
        }

        Vector3& mul(float x, float y, float z) {
            m_x *= x;
            m_y *= y;
            m_z *= z;
            return *this;
        }

        void setX(float x) {
            m_x = x;
        }

        void setY(float y) {
            m_y = y;
        }

        void setZ(float z) {
            m_z = z;
        }
    };

    class Vector4 {
    private:
        float m_x;
        float m_y;
        float m_z;
        float m_w;

    public:
        Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) :
            m_x(x), m_y(y), m_z(z), m_w(w)
        {
        }

        Vector2 getXY() const {
            return Vector2(m_x, m_y);
        }

        Vector3 getXYZ() const {
            return Vector3(m_x, m_y, m_z);
        }

        float getX() const {
            return m_x;
        }

        float getY() const {
            return m_y;
        }

        float getZ() const {
            return m_z;
        }

        float getW() const {
            return m_w;
        }

        Vector4& set(float x, float y, float z, float w) {
            m_x = x;
            m_y = y;
            m_z = z;
            m_w = w;
            return *this;
        }

        Vector4& add(float x, float y, float z, float w) {
            m_x += x;
            m_y += y;
            m_z += z;
            m_w += w;
            return *this;
        }

        Vector4& mul(float x, float y, float z, float w) {
            m_x *= x;
            m_y *= y;
            m_z *= z;
            m_w *= w;
            return *this;
        }

        void setX(float x) {
            m_x = x;
        }

        void setY(float y) {
            m_y = y;
        }

        void setZ(float z) {
            m_z = z;
        }

        void setW(float w) {
            m_w = w;
        }
    };
}
