#pragma once

namespace Orpheus::Math {
    class Color {
    private:
        float m_r;
        float m_g;
        float m_b;
        float m_a;

    public:
        static Color fromHSV(float h, float s, float v, float a = 1.0f);

        Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);

        float getR() const;
        float getG() const;
        float getB() const;
        float getA() const;

        Color& set(float r, float g, float b, float a);
        Color& set(const Color& color);
        void setR(float r);
        void setG(float g);
        void setB(float b);
        void setA(float a);
    };
}
