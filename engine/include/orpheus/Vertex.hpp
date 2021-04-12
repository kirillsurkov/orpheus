#pragma once

#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace Orpheus::Vertex {
    class Buffer {
    private:
        std::vector<float> m_data;
        std::size_t m_elements;

    public:
        Buffer(std::size_t elements) :
            m_elements(elements)
        {
            if (elements == 0) {
                throw std::runtime_error("Trying to create buffer with elements = 0");
            }
        }

        template<class... Args>
        void addPoint(Args&&... args) {
            if (sizeof...(args) != m_elements) {
                throw std::runtime_error("Wrong elements count");
            }
            (m_data.push_back(std::forward<Args>(args)), ...);
        }

        std::size_t getElements() const {
            return m_elements;
        }

        const std::vector<float>& getData() const {
            return m_data;
        }

        void clearCpuData() {
            m_data.clear();
        }
    };

    class BufferCache {
    private:
        std::unordered_map<std::string, Buffer> m_cache;

    public:
        Buffer& add(const std::string& key, std::size_t elements) {
            if (m_cache.find(key) != m_cache.end()) {
                throw std::runtime_error("Buffer '" + key + "' already exists!");
            }
            return m_cache.try_emplace(key, elements).first->second;
        }

        const Buffer& get(const std::string& key) const {
            auto it = m_cache.find(key);
            if (it == m_cache.end()) {
                throw std::runtime_error("Buffer '" + key + "' not found");
            }
            return it->second;
        }
    };

    class Attrib {
    private:
        std::size_t m_layout;
        const Buffer& m_buffer;

    public:
        Attrib(std::size_t layout, const Buffer& buffer) :
            m_layout(layout),
            m_buffer(buffer)
        {
        }

        std::size_t getLayout() const {
            return m_layout;
        }

        const Buffer& getBuffer() const {
            return m_buffer;
        }
    };

    class Vertices {
    private:
        std::vector<Attrib> m_attribs;

    public:
        const std::vector<Attrib>& getAttribs() const {
            return m_attribs;
        }

        Attrib& addAttrib(std::size_t layout, const Buffer& buffer) {
            m_attribs.emplace_back(layout, buffer);
            return m_attribs.back();
        }
    };
}
