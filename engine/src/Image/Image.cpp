#include "orpheus/Image/Image.hpp"
#include "orpheus/Image/loaders/LoaderPNG.hpp"

Orpheus::Image::Image Orpheus::Image::Image::loadPNG(const std::string& path) {
    Image self;
    Loader::PNG::load(path, self.m_width, self.m_height, self.m_data);
    return self;
}

const std::vector<float>& Orpheus::Image::Image::getData() const {
    return m_data;
}

std::size_t Orpheus::Image::Image::getWidth() const {
    return m_width;
}

std::size_t Orpheus::Image::Image::getHeight() const {
    return m_height;
}
