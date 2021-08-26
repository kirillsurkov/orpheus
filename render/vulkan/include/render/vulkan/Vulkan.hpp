#pragma once

#include <orpheus/interface/IRender.hpp>

namespace orpheus::render::vulkan {
    class Vulkan : public orpheus::interface::IRender {
    public:
        Vulkan(const std::shared_ptr<interface::IMath>& math);

        virtual void init()                                    override;
        virtual void startFrame()                              override;
        virtual void endFrame()                                override;

        virtual void  setSSBO(render::SsboId ssbo)             override;
        virtual void* ssboMapBuffer()                          override;
        virtual void  ssboUnmapBuffer()                        override;
        virtual void  ssboSetSize(std::uint32_t size)          override;

        virtual void setProjection(const math::Matrix4x4& mat) override;
        virtual void setView(const math::Matrix4x4& mat)       override;
        virtual void setModel(const math::Matrix4x4& mat)      override;
        virtual void clear(float r, float g, float b, float a) override;
        virtual void drawCube()                                override;
        virtual void drawSphere()                              override;
        virtual void drawCylinder()                            override;
        virtual void drawPlane()                               override;
        virtual void drawBumpy()                               override;

        virtual void setMaterial(const render::material::FlatColor& material) override;
        virtual void setMaterial(const render::material::GGX&       material) override;
    };
}
