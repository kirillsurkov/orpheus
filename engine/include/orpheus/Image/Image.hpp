#pragma once

#include <vector>
#include <memory>

namespace Orpheus::Image {
    class Image {
    private:
        std::size_t m_width;
        std::size_t m_height;
        std::vector<float> m_data;

        Image() {}

    public:
        static Image loadPNG(const std::string& path);

        std::size_t getWidth() const;
        std::size_t getHeight() const;
        const std::vector<float>& getData() const;
    };
}
