#pragma once

#include "orpheus/Command/Render/RenderCommand.hpp"

#include <vector>

namespace Orpheus::Command::Render {
    class RenderCommandVertices : public RenderCommand {
    public:
        class AttribArray {
        private:
            unsigned int m_layout;
            unsigned int m_elements;
            std::vector<float> m_array;

        public:
            AttribArray(unsigned int layout, unsigned int elements) :
                m_layout(layout),
                m_elements(elements)
            {
            }

            unsigned int getLayout() const {
                return m_layout;
            }

            unsigned int getElements() const {
                return m_elements;
            }

            const std::vector<float> getArray() const {
                return m_array;
            }

            template<class... Args>
            void addPoint(Args&&... args) {
                if (sizeof...(args) != m_elements) {
                    throw std::runtime_error("Wrong elements count");
                }
                (m_array.push_back(args), ...);
            }

            void clearCpuData() {
                m_array.clear();
            }
        };

    private:
        std::vector<std::shared_ptr<AttribArray>> m_attribs;

    public:
        RenderCommandVertices() {
        }

        virtual std::string getName() const override {
            return "RenderCommandVertices";
        }

        const std::vector<std::shared_ptr<AttribArray>>& getAttribs() const {
            return m_attribs;
        }

        const std::shared_ptr<AttribArray>& addAttrib(unsigned int layout, unsigned int elements) {
            m_attribs.push_back(std::make_shared<AttribArray>(layout, elements));
            return m_attribs.back();
        }
    };
}
