#pragma once

#include <orpheus/interface/IRender.hpp>

namespace orpheus::render::vulkan {
    class Vulkan : public orpheus::interface::IRender {
    public:
		virtual void clear(float r, float g, float b, float a) override;
		virtual void drawCube()                                override;
    };
}
