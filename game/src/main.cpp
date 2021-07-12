#include <SDL2/SDL.h>

#include <orpheus/Engine.hpp>

#include <math/cglm/CGLM.hpp>
#include <physics/physx/PhysX.hpp>
#include <physics/bullet/Bullet.hpp>
#include <render/opengl/OpenGL.hpp>

#include <unordered_map>
#include <vector>
#include <iostream>

class EntityCube : public orpheus::Entity {
private:
    orpheus::interface::math::Matrix4x4 m_model;
    orpheus::interface::IPhysics::State& m_cubeState;

public:
    EntityCube(orpheus::Entity& base, orpheus::Scene& scene) :
        orpheus::Entity(base),
        m_cubeState(scene.addBody(orpheus::interface::IPhysics::Shape::Cube))
    {
        m_math->translate(m_model, 1, -1, -10);
        m_math->scale(m_model, 0.5f, 0.5f, 0.5f);
    }

    virtual void update(float delta) override {
        m_math->rotate(m_model, 0, 1, 0, delta);
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->drawCube();
    }
};

class EntitySphere : public orpheus::Entity {
private:
    orpheus::interface::math::Matrix4x4 m_model;
    orpheus::interface::IPhysics::State& m_sphereState;

public:
    EntitySphere(orpheus::Entity& base, orpheus::Scene& scene) :
        orpheus::Entity(base),
        m_sphereState(scene.addBody(orpheus::interface::IPhysics::Shape::Sphere))
    {
        m_math->translate(m_model, -1, 1, -10);
        m_math->scale(m_model, 0.5f, 0.5f, 0.5f);
    }

    virtual void update(float delta) override {
        m_math->rotate(m_model, 0, 1, 0, delta);
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->drawSphere();
    }
};

class TestScene : public orpheus::Scene {
public:
    TestScene(const std::shared_ptr<orpheus::interface::IMath>& math,
              const std::shared_ptr<orpheus::interface::IPhysics>& physics) :
        orpheus::Scene(math, physics)
    {
        addEntity<EntityCube>();
        addEntity<EntitySphere>();
    }
};

orpheus::interface::IInput::Key keySdl(SDL_Scancode key) {
    switch (key) {
    case SDL_SCANCODE_ESCAPE: return orpheus::interface::IInput::Key::ESC;

    case SDL_SCANCODE_F1:     return orpheus::interface::IInput::Key::F1;
    case SDL_SCANCODE_F2:     return orpheus::interface::IInput::Key::F2;
    case SDL_SCANCODE_F3:     return orpheus::interface::IInput::Key::F3;
    case SDL_SCANCODE_F4:     return orpheus::interface::IInput::Key::F4;
    case SDL_SCANCODE_F5:     return orpheus::interface::IInput::Key::F5;
    case SDL_SCANCODE_F6:     return orpheus::interface::IInput::Key::F6;
    case SDL_SCANCODE_F7:     return orpheus::interface::IInput::Key::F7;
    case SDL_SCANCODE_F8:     return orpheus::interface::IInput::Key::F8;
    case SDL_SCANCODE_F9:     return orpheus::interface::IInput::Key::F9;
    case SDL_SCANCODE_F10:    return orpheus::interface::IInput::Key::F10;
    case SDL_SCANCODE_F11:    return orpheus::interface::IInput::Key::F11;
    case SDL_SCANCODE_F12:    return orpheus::interface::IInput::Key::F12;

    case SDL_SCANCODE_1:      return orpheus::interface::IInput::Key::N1;
    case SDL_SCANCODE_2:      return orpheus::interface::IInput::Key::N2;
    case SDL_SCANCODE_3:      return orpheus::interface::IInput::Key::N3;
    case SDL_SCANCODE_4:      return orpheus::interface::IInput::Key::N4;
    case SDL_SCANCODE_5:      return orpheus::interface::IInput::Key::N5;
    case SDL_SCANCODE_6:      return orpheus::interface::IInput::Key::N6;
    case SDL_SCANCODE_7:      return orpheus::interface::IInput::Key::N7;
    case SDL_SCANCODE_8:      return orpheus::interface::IInput::Key::N8;
    case SDL_SCANCODE_9:      return orpheus::interface::IInput::Key::N9;
    case SDL_SCANCODE_0:      return orpheus::interface::IInput::Key::N0;

    case SDL_SCANCODE_KP_1:   return orpheus::interface::IInput::Key::NP1;
    case SDL_SCANCODE_KP_2:   return orpheus::interface::IInput::Key::NP2;
    case SDL_SCANCODE_KP_3:   return orpheus::interface::IInput::Key::NP3;
    case SDL_SCANCODE_KP_4:   return orpheus::interface::IInput::Key::NP4;
    case SDL_SCANCODE_KP_5:   return orpheus::interface::IInput::Key::NP5;
    case SDL_SCANCODE_KP_6:   return orpheus::interface::IInput::Key::NP6;
    case SDL_SCANCODE_KP_7:   return orpheus::interface::IInput::Key::NP7;
    case SDL_SCANCODE_KP_8:   return orpheus::interface::IInput::Key::NP8;
    case SDL_SCANCODE_KP_9:   return orpheus::interface::IInput::Key::NP9;
    case SDL_SCANCODE_KP_0:   return orpheus::interface::IInput::Key::NP0;

    case SDL_SCANCODE_Q:      return orpheus::interface::IInput::Key::Q;
    case SDL_SCANCODE_W:      return orpheus::interface::IInput::Key::W;
    case SDL_SCANCODE_E:      return orpheus::interface::IInput::Key::E;
    case SDL_SCANCODE_R:      return orpheus::interface::IInput::Key::R;
    case SDL_SCANCODE_T:      return orpheus::interface::IInput::Key::T;
    case SDL_SCANCODE_Y:      return orpheus::interface::IInput::Key::Y;
    case SDL_SCANCODE_U:      return orpheus::interface::IInput::Key::U;
    case SDL_SCANCODE_I:      return orpheus::interface::IInput::Key::I;
    case SDL_SCANCODE_O:      return orpheus::interface::IInput::Key::O;
    case SDL_SCANCODE_P:      return orpheus::interface::IInput::Key::P;
    case SDL_SCANCODE_A:      return orpheus::interface::IInput::Key::A;
    case SDL_SCANCODE_S:      return orpheus::interface::IInput::Key::S;
    case SDL_SCANCODE_D:      return orpheus::interface::IInput::Key::D;
    case SDL_SCANCODE_F:      return orpheus::interface::IInput::Key::F;
    case SDL_SCANCODE_G:      return orpheus::interface::IInput::Key::G;
    case SDL_SCANCODE_H:      return orpheus::interface::IInput::Key::H;
    case SDL_SCANCODE_J:      return orpheus::interface::IInput::Key::J;
    case SDL_SCANCODE_K:      return orpheus::interface::IInput::Key::K;
    case SDL_SCANCODE_L:      return orpheus::interface::IInput::Key::L;
    case SDL_SCANCODE_Z:      return orpheus::interface::IInput::Key::Z;
    case SDL_SCANCODE_X:      return orpheus::interface::IInput::Key::X;
    case SDL_SCANCODE_C:      return orpheus::interface::IInput::Key::C;
    case SDL_SCANCODE_V:      return orpheus::interface::IInput::Key::V;
    case SDL_SCANCODE_B:      return orpheus::interface::IInput::Key::B;
    case SDL_SCANCODE_N:      return orpheus::interface::IInput::Key::N;
    case SDL_SCANCODE_M:      return orpheus::interface::IInput::Key::M;

    case SDL_SCANCODE_SPACE:  return orpheus::interface::IInput::Key::SPACE;
    case SDL_SCANCODE_RETURN: return orpheus::interface::IInput::Key::ENTER;

    case SDL_SCANCODE_UP:     return orpheus::interface::IInput::Key::UP;
    case SDL_SCANCODE_DOWN:   return orpheus::interface::IInput::Key::DOWN;
    case SDL_SCANCODE_LEFT:   return orpheus::interface::IInput::Key::LEFT;
    case SDL_SCANCODE_RIGHT:  return orpheus::interface::IInput::Key::RIGHT;

    case SDL_SCANCODE_LCTRL:  return orpheus::interface::IInput::Key::L_CTRL;
    case SDL_SCANCODE_LSHIFT: return orpheus::interface::IInput::Key::L_SHIFT;
    case SDL_SCANCODE_LALT:   return orpheus::interface::IInput::Key::L_ALT;

    case SDL_SCANCODE_RCTRL:  return orpheus::interface::IInput::Key::R_CTRL;
    case SDL_SCANCODE_RSHIFT: return orpheus::interface::IInput::Key::R_SHIFT;
    case SDL_SCANCODE_RALT:   return orpheus::interface::IInput::Key::R_ALT;

    /*LMB, MMB, RMB*/

    default: return orpheus::interface::IInput::Key::UNKNOWN;
    }
}

class InputSDL : public orpheus::interface::IInput {
private:
    bool                                        m_quit;
    std::vector<std::pair<SDL_Scancode, bool>>  m_keys;
    std::unordered_multimap<SDL_Scancode, bool> m_keysPressed;

    void onKey(SDL_Scancode key, bool down) {
        auto it = m_keysPressed.find(key);
        if (it == m_keysPressed.end() || it->second != down) {
            m_keys.push_back({key, down});
            m_keysPressed.emplace(key, down);
        }
    }

public:
    InputSDL() :
        m_quit(false)
    { }

    virtual void update() override {
        m_keys.clear();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                m_quit = true;
                break;
            }
            case SDL_KEYDOWN: {
                onKey(event.key.keysym.scancode, true);
                break;
            }
            case SDL_KEYUP: {
                onKey(event.key.keysym.scancode, false);
                break;
            }
            }
        }
    }

    virtual void apply(const orpheus::interface::IInput::Listener& listener) const override {
        for (const auto& pair : m_keys) {
            listener(keySdl(pair.first), pair.second ? orpheus::interface::IInput::State::Down : orpheus::interface::IInput::State::Up);
        }
    }

    virtual bool isQuit() const override {
        return m_quit;
    }
};

class RenderContextSDLOpenGL : public orpheus::interface::IRenderContext {
private:
    SDL_GLContext m_glContext;

public:
    RenderContextSDLOpenGL(SDL_Window* window) {
        m_glContext = SDL_GL_CreateContext(window);
    }

    ~RenderContextSDLOpenGL() {
        SDL_GL_DeleteContext(m_glContext);
    }
};

class WindowSDLOpenGL : public orpheus::interface::IWindow {
private:
    SDL_Window*   m_window;

public:
    virtual void create(const std::string& title, std::uint32_t width, std::uint32_t height) override {
        m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    }

    virtual std::shared_ptr<orpheus::interface::IRenderContext> createContext() override {
        return std::make_shared<RenderContextSDLOpenGL>(m_window);
    }

    virtual void destroy() override {
        SDL_DestroyWindow(m_window);
    }

    virtual void swapBuffers() override {
        SDL_GL_SwapWindow(m_window);
    }
};

int main() {
    auto window  = std::make_shared<WindowSDLOpenGL>();
    auto math    = std::make_shared<orpheus::math::cglm::CGLM>();
    auto render  = std::make_shared<orpheus::render::opengl::OpenGL>(math);
    auto input   = std::make_shared<InputSDL>();
    auto physics = std::make_shared<orpheus::physics::bullet::Bullet>();
    auto scene   = std::make_shared<TestScene>(math, physics);

    orpheus::Engine(window, render, input, physics, math, scene).run();

    return 0;
}
