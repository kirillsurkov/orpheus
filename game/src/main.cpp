#include <SDL2/SDL.h>

#include <orpheus/Engine.hpp>

#include <math/cglm/CGLM.hpp>
#include <physics/physx/PhysX.hpp>
#include <render/opengl/OpenGL.hpp>

#include <unordered_map>
#include <vector>
#include <iostream>

class EntityCube : public orpheus::Entity {
private:
    orpheus::physics::ID m_cubeId;
    orpheus::math::Matrix4x4 m_model;

public:
    EntityCube(orpheus::Entity& base, orpheus::Scene& scene, float x, float z) :
        orpheus::Entity(base)
    {
        m_cubeId = scene.addBody(orpheus::physics::Description{
                                     orpheus::physics::Shape::Cube,
                                     orpheus::physics::Type::Static,
                                     orpheus::math::Vector3{x, -3.0f, z}
                                 });
    }

    virtual void update(float delta) override {
        m_model = m_physics->getState(m_cubeId).transform;
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->drawCube();
    }
};

class EntitySphere : public orpheus::Entity {
private:
    orpheus::physics::ID m_sphereId;
    orpheus::math::Matrix4x4 m_model;

public:
    EntitySphere(orpheus::Entity& base, orpheus::Scene& scene, float x, float y) :
        orpheus::Entity(base)
    {
        m_sphereId = scene.addBody(orpheus::physics::Description{
                                       orpheus::physics::Shape::Sphere,
                                       orpheus::physics::Type::Dynamic,
                                       orpheus::math::Vector3{x, y, -20.0f}
                                   });
    }

    virtual void update(float delta) override {
        m_model = m_physics->getState(m_sphereId).transform;
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->drawSphere();
    }
};

class EntityPlayer : public orpheus::Entity {
private:
    orpheus::physics::ID m_capsuleId;
    orpheus::math::Vector3 m_dirForward;
    orpheus::math::Vector3 m_dirRight;
    orpheus::math::Vector3 m_position;
    bool m_forward  = false;
    bool m_backward = false;
    bool m_left     = false;
    bool m_right    = false;
    orpheus::math::Matrix4x4 m_model;

public:
    EntityPlayer(orpheus::Entity& base, orpheus::Scene& scene) :
        orpheus::Entity(base)
    {
        m_capsuleId = scene.addBody(orpheus::physics::Description{
                                        orpheus::physics::Shape::Capsule,
                                        orpheus::physics::Type::Dynamic,
                                        orpheus::math::Vector3{-4.5f, 5, -20.0f}
                                    });
    }

    void setDirectionVectors(const orpheus::math::Vector3& forward, const orpheus::math::Vector3& right) {
        m_dirForward = forward;
        m_dirForward.y() = 0;

        m_dirRight = right;
        m_dirRight.y() = 0;
    }

    void forward(bool enable)  { m_forward = enable; }
    void backward(bool enable) { m_backward = enable; }
    void left(bool enable)     { m_left = enable; }
    void right(bool enable)    { m_right = enable; }

    const orpheus::math::Vector3& getPosition() const {
        return m_position;
    }

    virtual void update(float delta) override {
        orpheus::math::Vector4 position = {0.0f, 0.0f, 0.0f, 1.0f};
        m_math->mul(position, m_physics->getState(m_capsuleId).transform, position);
        m_position = {position.x(), position.y(), position.z()};
        m_position.y() += 3.0f;

        orpheus::math::Vector3 direction;
        if (m_forward)  m_math->add(direction, direction, m_dirForward);
        if (m_backward) m_math->sub(direction, direction, m_dirForward);
        if (m_left)     m_math->sub(direction, direction, m_dirRight);
        if (m_right)    m_math->add(direction, direction, m_dirRight);
        if (direction.x() || direction.y() || direction.z()) {
            m_math->normalize(direction);
            m_math->mul(direction, direction, 5.0f);
        }

        orpheus::math::Vector3 velocity = m_physics->getLinearVelocity(m_capsuleId);
        direction.y() += velocity.y();

        m_physics->setLinearVelocity(m_capsuleId, direction);

        m_model = m_physics->getState(m_capsuleId).transform;
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->drawCylinder();

        m_math->translate(m_model, 0.0f, 1.0f, 0.0f);
        render->setModel(m_model);
        render->drawSphere();

        m_math->translate(m_model, 0.0f, -2.0f, 0.0f);
        render->setModel(m_model);
        render->drawSphere();
    }
};

class TestScene : public orpheus::Scene {
private:
    std::shared_ptr<EntityPlayer> m_player;

public:
    TestScene(const std::shared_ptr<orpheus::interface::IMath>& math,
              const std::shared_ptr<orpheus::interface::IInput>& input,
              const std::shared_ptr<orpheus::interface::IPhysics>& physics) :
        orpheus::Scene(math, input, physics)
    {
        for (int x = 0; x < 10; x++) {
            for (int z = 0; z < 10; z++) {
                addEntity<EntityCube>((x - 5) * 2.1f, (z - 5) * 2.1f - 20);
            }
        }

        addEntity<EntitySphere>(2.5f, 1.0f);
        addEntity<EntitySphere>(2.6f, 3.0f);

        m_player = addEntity<EntityPlayer>();

        bindKey(orpheus::input::Key::W, [&](orpheus::input::State state) { m_player->forward(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::S, [&](orpheus::input::State state) { m_player->backward(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::A, [&](orpheus::input::State state) { m_player->left(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::D, [&](orpheus::input::State state) { m_player->right(state == orpheus::input::State::Down); });
    }

    virtual void userUpdate(float delta) override {
        m_camera.setPosition(m_player->getPosition());
        m_player->setDirectionVectors(m_camera.getForward(), m_camera.getRight());
    }
};

class EntityAreaLight : public orpheus::Entity {
private:
    orpheus::math::Matrix4x4             m_model;
    orpheus::render::material::FlatColor m_material;
    std::vector<orpheus::math::Vector3>  m_points;

public:
    EntityAreaLight(orpheus::Entity& base, orpheus::Scene& scene, float x, float y, float z) :
        orpheus::Entity(base)
    {
        m_math->translate(m_model, x, y + 2.0f, z);
        m_math->scale(m_model, 2.0f, 2.0f, 1.0f);
        m_math->rotate(m_model, 1.0f, 0.0f, 0.0f, 3.141592653f / 2.0f);

        m_material.color = {1.0f, 1.0f, 1.0f};

        m_points.insert(m_points.end(), {x - 2.0f, y,        z});
        m_points.insert(m_points.end(), {x + 2.0f, y,        z});
        m_points.insert(m_points.end(), {x + 2.0f, y + 4.0f, z});
        m_points.insert(m_points.end(), {x - 2.0f, y + 4.0f, z});
    }

    void setColor(float r, float g, float b) {
        m_material.color.x() = r;
        m_material.color.y() = g;
        m_material.color.z() = b;
    }

    const std::vector<orpheus::math::Vector3>& getPoints() const {
        return m_points;
    }

    const orpheus::math::Vector3& getColor() const {
        return m_material.color;
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->setMaterial(m_material);
        render->drawPlane();
    }
};

class EntityReflectFloor : public orpheus::Entity {
private:
    orpheus::math::Matrix4x4       m_model;
    orpheus::render::material::GGX m_material;

public:
    EntityReflectFloor(orpheus::Entity& base, orpheus::Scene& scene) :
        orpheus::Entity(base)
    {
        m_math->scale(m_model, 100.0f, 5.0f, 100.0f);

        m_material.roughness = 0.2f;
    }

    void addAreaLight(const std::shared_ptr<EntityAreaLight>& areaLight) {
        std::size_t offset = m_material.lightPoints.size() / 3;
        std::size_t count = areaLight->getPoints().size();

        m_material.lightColors.insert(m_material.lightColors.end(), {areaLight->getColor().x(), areaLight->getColor().y(), areaLight->getColor().z()});

        for (const auto& point : areaLight->getPoints()) {
            m_material.lightPoints.insert(m_material.lightPoints.end(), {point.x(), point.y(), point.z()});
        }

        for (std::size_t i = 0; i < count; i++) {
            m_material.lightIndices.push_back(offset + i);
            m_material.lightIndices.push_back(offset + (i + 1) % count);
        }
    }

    virtual void update(float delta) override {
        /*m_material.roughness -= delta * 0.1f;
        if (m_material.roughness <= 0.0f) {
            m_material.roughness = 1.0f;
        }*/
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->setMaterial(m_material);
        render->drawBumpy();
    }
};

class TestScene2 : public orpheus::Scene {
public:
    TestScene2(const std::shared_ptr<orpheus::interface::IMath>& math,
               const std::shared_ptr<orpheus::interface::IInput>& input,
               const std::shared_ptr<orpheus::interface::IPhysics>& physics) :
        orpheus::Scene(math, input, physics)
    {
        m_camera.setPosition(orpheus::math::Vector3{0.0f, 5.0f, 0.0f});
        auto floor = addEntity<EntityReflectFloor>();

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 7; j++) {
                auto light = addEntity<EntityAreaLight>(-2.5f + i * 7.0f, 0.5f, -j * 10.0f);
                /*if (rand() % 2) {
                    light->setColor(1.0f, 0.0f, 0.0f);
                } else {
                    light->setColor(0.0f, 0.0f, 1.0f);
                }*/
                light->setColor(i / 10.0f, 1.0f - (i * 7 + j) / 70.0f, j / 7.0f);
                floor->addAreaLight(light);
            }
        }

        bindKey(orpheus::input::Key::W, [&](orpheus::input::State state) { m_camera.flyForward(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::S, [&](orpheus::input::State state) { m_camera.flyBackward(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::A, [&](orpheus::input::State state) { m_camera.flyLeft(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::D, [&](orpheus::input::State state) { m_camera.flyRight(state == orpheus::input::State::Down); });
    }
};

orpheus::input::Key keySdl(SDL_Scancode key) {
    switch (key) {
    case SDL_SCANCODE_ESCAPE: return orpheus::input::Key::ESC;

    case SDL_SCANCODE_F1:     return orpheus::input::Key::F1;
    case SDL_SCANCODE_F2:     return orpheus::input::Key::F2;
    case SDL_SCANCODE_F3:     return orpheus::input::Key::F3;
    case SDL_SCANCODE_F4:     return orpheus::input::Key::F4;
    case SDL_SCANCODE_F5:     return orpheus::input::Key::F5;
    case SDL_SCANCODE_F6:     return orpheus::input::Key::F6;
    case SDL_SCANCODE_F7:     return orpheus::input::Key::F7;
    case SDL_SCANCODE_F8:     return orpheus::input::Key::F8;
    case SDL_SCANCODE_F9:     return orpheus::input::Key::F9;
    case SDL_SCANCODE_F10:    return orpheus::input::Key::F10;
    case SDL_SCANCODE_F11:    return orpheus::input::Key::F11;
    case SDL_SCANCODE_F12:    return orpheus::input::Key::F12;

    case SDL_SCANCODE_1:      return orpheus::input::Key::N1;
    case SDL_SCANCODE_2:      return orpheus::input::Key::N2;
    case SDL_SCANCODE_3:      return orpheus::input::Key::N3;
    case SDL_SCANCODE_4:      return orpheus::input::Key::N4;
    case SDL_SCANCODE_5:      return orpheus::input::Key::N5;
    case SDL_SCANCODE_6:      return orpheus::input::Key::N6;
    case SDL_SCANCODE_7:      return orpheus::input::Key::N7;
    case SDL_SCANCODE_8:      return orpheus::input::Key::N8;
    case SDL_SCANCODE_9:      return orpheus::input::Key::N9;
    case SDL_SCANCODE_0:      return orpheus::input::Key::N0;

    case SDL_SCANCODE_KP_1:   return orpheus::input::Key::NP1;
    case SDL_SCANCODE_KP_2:   return orpheus::input::Key::NP2;
    case SDL_SCANCODE_KP_3:   return orpheus::input::Key::NP3;
    case SDL_SCANCODE_KP_4:   return orpheus::input::Key::NP4;
    case SDL_SCANCODE_KP_5:   return orpheus::input::Key::NP5;
    case SDL_SCANCODE_KP_6:   return orpheus::input::Key::NP6;
    case SDL_SCANCODE_KP_7:   return orpheus::input::Key::NP7;
    case SDL_SCANCODE_KP_8:   return orpheus::input::Key::NP8;
    case SDL_SCANCODE_KP_9:   return orpheus::input::Key::NP9;
    case SDL_SCANCODE_KP_0:   return orpheus::input::Key::NP0;

    case SDL_SCANCODE_Q:      return orpheus::input::Key::Q;
    case SDL_SCANCODE_W:      return orpheus::input::Key::W;
    case SDL_SCANCODE_E:      return orpheus::input::Key::E;
    case SDL_SCANCODE_R:      return orpheus::input::Key::R;
    case SDL_SCANCODE_T:      return orpheus::input::Key::T;
    case SDL_SCANCODE_Y:      return orpheus::input::Key::Y;
    case SDL_SCANCODE_U:      return orpheus::input::Key::U;
    case SDL_SCANCODE_I:      return orpheus::input::Key::I;
    case SDL_SCANCODE_O:      return orpheus::input::Key::O;
    case SDL_SCANCODE_P:      return orpheus::input::Key::P;
    case SDL_SCANCODE_A:      return orpheus::input::Key::A;
    case SDL_SCANCODE_S:      return orpheus::input::Key::S;
    case SDL_SCANCODE_D:      return orpheus::input::Key::D;
    case SDL_SCANCODE_F:      return orpheus::input::Key::F;
    case SDL_SCANCODE_G:      return orpheus::input::Key::G;
    case SDL_SCANCODE_H:      return orpheus::input::Key::H;
    case SDL_SCANCODE_J:      return orpheus::input::Key::J;
    case SDL_SCANCODE_K:      return orpheus::input::Key::K;
    case SDL_SCANCODE_L:      return orpheus::input::Key::L;
    case SDL_SCANCODE_Z:      return orpheus::input::Key::Z;
    case SDL_SCANCODE_X:      return orpheus::input::Key::X;
    case SDL_SCANCODE_C:      return orpheus::input::Key::C;
    case SDL_SCANCODE_V:      return orpheus::input::Key::V;
    case SDL_SCANCODE_B:      return orpheus::input::Key::B;
    case SDL_SCANCODE_N:      return orpheus::input::Key::N;
    case SDL_SCANCODE_M:      return orpheus::input::Key::M;

    case SDL_SCANCODE_SPACE:  return orpheus::input::Key::SPACE;
    case SDL_SCANCODE_RETURN: return orpheus::input::Key::ENTER;

    case SDL_SCANCODE_UP:     return orpheus::input::Key::UP;
    case SDL_SCANCODE_DOWN:   return orpheus::input::Key::DOWN;
    case SDL_SCANCODE_LEFT:   return orpheus::input::Key::LEFT;
    case SDL_SCANCODE_RIGHT:  return orpheus::input::Key::RIGHT;

    case SDL_SCANCODE_LCTRL:  return orpheus::input::Key::L_CTRL;
    case SDL_SCANCODE_LSHIFT: return orpheus::input::Key::L_SHIFT;
    case SDL_SCANCODE_LALT:   return orpheus::input::Key::L_ALT;

    case SDL_SCANCODE_RCTRL:  return orpheus::input::Key::R_CTRL;
    case SDL_SCANCODE_RSHIFT: return orpheus::input::Key::R_SHIFT;
    case SDL_SCANCODE_RALT:   return orpheus::input::Key::R_ALT;

    /*LMB, MMB, RMB*/

    default: return orpheus::input::Key::UNKNOWN;
    }
}

class InputSDL : public orpheus::interface::IInput {
private:
    bool                                        m_quit;
    orpheus::input::Keys                        m_keys;
    std::unordered_multimap<SDL_Scancode, bool> m_keysPressed;
    orpheus::math::Vector2                      m_realMouseXY;
    orpheus::math::Vector2                      m_smoothMouseXY;

    void onKey(SDL_Scancode key, bool down) {
        auto it = m_keysPressed.find(key);
        if (it == m_keysPressed.end() || it->second != down) {
            m_keys.push_back({keySdl(key), down ? orpheus::input::State::Down : orpheus::input::State::Up});
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
            case SDL_MOUSEMOTION: {
                m_realMouseXY.x() += event.motion.xrel;
                m_realMouseXY.y() += event.motion.yrel;
                break;
            }
            }
        }

        float d = 1.0f - exp(log(0.5f) * 100.0f * 1.0f / 60.0f);
        m_smoothMouseXY.x() += (m_realMouseXY.x() - m_smoothMouseXY.x()) * d;
        m_smoothMouseXY.y() += (m_realMouseXY.y() - m_smoothMouseXY.y()) * d;
    }

    virtual const orpheus::input::Keys& getKeys() const override {
        return m_keys;
    }

    virtual orpheus::math::Vector2 getMouseDxDy() const override {
        return {-m_realMouseXY.x() + m_smoothMouseXY.x(), -m_realMouseXY.y() + m_smoothMouseXY.y()};
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
        SDL_SetRelativeMouseMode(SDL_TRUE);
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
    auto physics = std::make_shared<orpheus::physics::physx::PhysX>();
    auto scene   = std::make_shared<TestScene2>(math, input, physics);

    orpheus::Engine(window, render, input, physics, math, scene).run();

    return 0;
}
