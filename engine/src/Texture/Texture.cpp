#include "orpheus/Texture/Texture.hpp"
#include "orpheus/Texture/loaders/LoaderPNG.hpp"

Orpheus::Texture::Texture Orpheus::Texture::Texture::loadPNG(const std::string& path) {
    Texture self;
    Loader::PNG::load(path, self.m_width, self.m_height, self.m_data);
    return self;
}

std::size_t Orpheus::Texture::Texture::getWidth() const {
    return m_width;
}

std::size_t Orpheus::Texture::Texture::getHeight() const {
    return m_height;
}

const std::vector<float>& Orpheus::Texture::Texture::getCpuData() const {
    return m_data;
}

void Orpheus::Texture::Texture::clearCpuData() const {
    m_data.clear();
}
