#pragma once

#include <vector>
#include <memory>

namespace Orpheus::Texture {
    class Texture {
    private:
        std::size_t m_width;
        std::size_t m_height;
        mutable std::vector<float> m_data;

        Texture() {}

    public:
        static Texture loadPNG(const std::string& path);

        std::size_t getWidth() const;
        std::size_t getHeight() const;
        const std::vector<float>& getCpuData() const;
        void clearCpuData() const;
    };
}
