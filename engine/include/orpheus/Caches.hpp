#pragma once

#include "orpheus/Vertex.hpp"

namespace Orpheus {
    class Caches {
    private:
        Vertex::BufferCache m_vertexBufferCache;

    public:
        Vertex::BufferCache& vertexBufferCache() {
            return m_vertexBufferCache;
        }
    };
}
