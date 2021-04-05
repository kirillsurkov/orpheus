#pragma once

#include "orpheus/Command/Render/Command.hpp"

#include <vector>

namespace Orpheus::Command::Render {
    class CommandVertices : public Command {
    public:
        class Buffer {
        private:
            std::vector<float> m_data;

        public:
            template<class... Args>
            void addPoint(Args&&... args) {
                (m_data.push_back(std::forward<Args>(args)), ...);
            }

            const std::vector<float>& getData() const {
                return m_data;
            }

            void clearCpuData() {
                m_data.clear();
            }
        };

        class AttribArray {
        private:
            unsigned int m_layout;
            unsigned int m_elements;
            Buffer m_buffer;

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

            const Buffer& getBuffer() const {
                return m_buffer;
            }

            template<class... Args>
            void addPoint(Args&&... args) {
                if (sizeof...(args) != m_elements) {
                    throw std::runtime_error("Wrong elements count");
                }
                m_buffer.addPoint(std::forward<Args>(args)...);
            }

            void clearCpuData() {
                m_buffer.clearCpuData();
            }
        };

    private:
        std::vector<std::shared_ptr<AttribArray>> m_attribs;

    public:
        virtual std::string getName() const override {
            return "CommandVertices";
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
