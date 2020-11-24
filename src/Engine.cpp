#include "Engine.hpp"
#include "GL/gl.h"

Engine::Engine(const WindowPtr& window, const RenderPtr& render) :
    m_window(window),
    m_render(render)
{
}

Engine::~Engine() {
}

void Engine::loop() {
    while (true) {
        glClearColor(1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_window->swapBuffers();
    }
}
