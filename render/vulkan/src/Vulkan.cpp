#include "render/vulkan/Vulkan.hpp"

#include <iostream>

namespace orpheus::render::vulkan {
    Vulkan::Vulkan(const std::shared_ptr<interface::IMath>& math) {
    }

    void Vulkan::init() {

    }

    void Vulkan::startFrame() {

    }

    void Vulkan::endFrame() {

    }

    void Vulkan::setSSBO(render::SsboId ssbo) {

    }

    void* Vulkan::ssboMapBuffer() {

    }

    void Vulkan::ssboUnmapBuffer() {

    }

    void Vulkan::ssboSetSize(std::uint32_t size) {

    }

    void Vulkan::setProjection(const math::Matrix4x4& mat) {

    }

    void Vulkan::setView(const math::Matrix4x4& mat) {

    }

    void Vulkan::setModel(const math::Matrix4x4& mat) {

    }

    void Vulkan::clear(float r, float g, float b, float a) {

    }

    void Vulkan::drawCube() {

    }

    void Vulkan::drawSphere() {

    }

    void Vulkan::drawCylinder() {

    }

    void Vulkan::drawPlane() {

    }

    void Vulkan::drawBumpy() {

    }

    void Vulkan::setMaterial(const render::material::FlatColor& material) {

    }

    void Vulkan::setMaterial(const render::material::GGX& material) {

    }
}
