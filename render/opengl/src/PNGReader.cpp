#include "render/opengl/PNGReader.hpp"

#include <fstream>
#include <png.h>

namespace orpheus::render::opengl {
    namespace {
        static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
            std::istream* from = reinterpret_cast<std::istream*>(png_get_io_ptr(png_ptr));
            if (!from->read(reinterpret_cast<char*>(data), length)) {
                png_error(png_ptr, "error reading");
            }
        }
    }

    void PNGReader::read(const std::string& filename, std::uint32_t& width, std::uint32_t& height, std::vector<unsigned char>& pixels) {
        width = 0;
        height = 0;
        pixels.clear();

        std::ifstream input(filename, std::ios_base::binary);
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

        png_set_read_fn(png, &input, user_read_data);

        if (!png) {
            throw std::runtime_error("PNGReader: cannot alloc read struct");
        }

        png_infop info = png_create_info_struct(png);
        if (!info) {
            png_destroy_read_struct(&png, nullptr, nullptr);
            throw std::runtime_error("PNGReader: cannot alloc info struct");
        }

        png_bytep* row_pointers = nullptr;
#ifdef _WIN32
        if (__builtin_setjmp(png_jmpbuf(png))) {
#else
        if (setjmp(png_jmpbuf(png))) {
#endif
            png_destroy_read_struct(&png, &info, nullptr);
            if (row_pointers != nullptr) delete[] row_pointers;
            pixels.clear();
            throw std::runtime_error("PNGReader: png interal error");
        }

        png_read_info(png, info);
        width = png_get_image_width(png, info);
        height = png_get_image_height(png, info);
        if (png_get_color_type(png, info) == PNG_COLOR_TYPE_PALETTE)
            png_set_palette_to_rgb(png);
        if (png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY || png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png);
        if (!(png_get_color_type(png, info) & PNG_COLOR_MASK_ALPHA))
            png_set_add_alpha(png, 0xFF, PNG_FILLER_AFTER);
        if (png_get_bit_depth(png, info) < 8)
            png_set_packing(png);
        if (png_get_bit_depth(png,info) == 16)
            png_set_strip_16(png);

        png_read_update_info(png, info);

        pixels.resize(width * height * 4);
        row_pointers = new png_bytep[height];
        for (std::uint32_t row = 0; row < height; row++) {
            row_pointers[height - 1 - row] = static_cast<png_bytep>(pixels.data() + row * width * 4);
        }
        png_read_image(png, row_pointers);

        png_destroy_read_struct(&png, &info, NULL);
        delete[] row_pointers;
    }
}
