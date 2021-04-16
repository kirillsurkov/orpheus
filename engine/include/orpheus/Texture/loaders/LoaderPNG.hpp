#pragma once

#include "orpheus/Texture/Texture.hpp"

#include <libpng/png.h>
#include <stdexcept>

namespace Orpheus::Texture::Loader {
    class PNG {
    public:
        static void load(const std::string& path, std::size_t& width, std::size_t& height, std::vector<float>& dest) {
            FILE* fp = fopen(path.c_str(), "rb");
            if (!fp) {
                throw std::runtime_error("Unable to open PNG image '" + path + "'");
            }

            png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

            if (!png) {
                throw std::runtime_error("png_create_read_struct returned nullptr");
            }

            png_infop info = png_create_info_struct(png);
            if (!info) {
                throw std::runtime_error("png_create_info_struct returned nullptr");
            }

            if (setjmp(png_jmpbuf(png))) {
                throw std::runtime_error("libpng performing non-local jump");
            }

            png_init_io(png, fp);

            png_read_info(png, info);

            width = png_get_image_width(png, info);
            height = png_get_image_height(png, info);
            std::size_t color_type = png_get_color_type(png, info);
            std::size_t bit_depth  = png_get_bit_depth(png, info);

            if (bit_depth == 16) {
                png_set_strip_16(png);
            }

            if (color_type == PNG_COLOR_TYPE_PALETTE) {
                png_set_palette_to_rgb(png);
            }

            if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
                png_set_expand_gray_1_2_4_to_8(png);
            }

            if (png_get_valid(png, info, PNG_INFO_tRNS)) {
                png_set_tRNS_to_alpha(png);
            }

            if (color_type == PNG_COLOR_TYPE_RGB ||
                color_type == PNG_COLOR_TYPE_GRAY ||
                color_type == PNG_COLOR_TYPE_PALETTE) {
                png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
            }

            if (color_type == PNG_COLOR_TYPE_GRAY ||
                color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
                png_set_gray_to_rgb(png);
            }

            png_read_update_info(png, info);

            std::size_t rowBytes = png_get_rowbytes(png, info);
            png_bytep* row_pointers = new png_bytep[height];
            for (std::size_t y = 0; y < height; y++) {
                row_pointers[y] = new png_byte[rowBytes];
            }

            png_read_image(png, row_pointers);

            dest.clear();
            dest.reserve(width * height * 4);
            for (std::size_t y = height; y > 0; y--) {
                png_bytep src = row_pointers[y - 1];
                for (std::size_t x = 0; x < width; x++) {
                    dest.push_back(src[x * 4 + 0] / 255.0f);
                    dest.push_back(src[x * 4 + 1] / 255.0f);
                    dest.push_back(src[x * 4 + 2] / 255.0f);
                    dest.push_back(src[x * 4 + 3] / 255.0f);
                }
                delete[] row_pointers[y - 1];
            }

            delete[] row_pointers;

            fclose(fp);

            png_destroy_read_struct(&png, &info, nullptr);
        }
    };
}
