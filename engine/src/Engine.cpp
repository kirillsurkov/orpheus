#include "orpheus/Engine.hpp"
#include <GL/gl.h>

Orpheus::Engine::Engine(const WindowPtr& window, const RenderPtr& render) :
    m_window(window),
    m_render(render)
{
}

Orpheus::Engine::~Engine() {
}

void Orpheus::Engine::loop() {
    while (true) {
        //glClearColor(1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_window->swapBuffers();
    }
}
