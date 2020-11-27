#include "orpheus/Engine.hpp"
#include <GL/gl.h>

Orpheus::Engine::Engine(const Window::WindowPtr& window, const Render::RenderPtr& render) :
    m_window(window),
    m_render(render)
{
    addScope("Engine");
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
