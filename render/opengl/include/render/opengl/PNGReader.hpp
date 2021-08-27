#pragma once

#include <string>
#include <vector>

namespace orpheus::render::opengl {
    class PNGReader {
    public:
        static void read(const std::string& filename, std::uint32_t& width, std::uint32_t& height, std::vector<unsigned char>& pixels);
    };
}
