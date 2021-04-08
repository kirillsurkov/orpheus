#include "orpheus/Image/Image.hpp"
#include "orpheus/Image/loaders/LoaderPNG.hpp"

Orpheus::Image::Image() {
}

Orpheus::Image Orpheus::Image::loadPNG(const std::string& path) {
    Image self;
    ImageLoader::PNG::load(path, self.m_width, self.m_height, self.m_data);
    return self;
}

const std::vector<float>& Orpheus::Image::getData() const {
    return m_data;
}

std::size_t Orpheus::Image::getWidth() const {
    return m_width;
}

std::size_t Orpheus::Image::getHeight() const {
    return m_height;
}
