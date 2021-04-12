#pragma once

#include "orpheus/Render/Command/Command.hpp"

namespace Orpheus::Render::Command {
    class ClearColor : public Command {
    private:
        float m_r;
        float m_g;
        float m_b;
        float m_a;

    public:
        ClearColor(float r, float g, float b, float a) :
            m_r(r), m_g(g), m_b(b), m_a(a)
        {
        }

        virtual std::string getName() const override {
            return "CommandClearColor";
        }

        void set(float r, float g, float b, float a) {
            m_r = r;
            m_g = g;
            m_b = b;
            m_a = a;
        }

        void setR(float r) {
            m_r = r;
        }

        void setG(float g) {
            m_g = g;
        }

        void setB(float b) {
            m_b = b;
        }

        void setA(float a) {
            m_a = a;
        }

        float getR() const {
            return m_r;
        }

        float getG() const {
            return m_g;
        }

        float getB() const {
            return m_b;
        }

        float getA() const {
            return m_a;
        }
    };
}
