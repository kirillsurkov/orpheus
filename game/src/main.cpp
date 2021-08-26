#include <SDL.h>
#include <SDL_vulkan.h>

#include <vulkan/vulkan.h>

#include <orpheus/Engine.hpp>

#include <math/cglm/CGLM.hpp>
//#include <physics/physx/PhysX.hpp>
#include <render/opengl/OpenGL.hpp>
#include <render/vulkan/Vulkan.hpp>

#include <unordered_map>
#include <vector>
#include <iostream>

/*class EntityCube : public orpheus::Entity {
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
};*/

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

        m_points.insert(m_points.end(), {-1.0f,  1.0f,  0.0f});
        m_points.insert(m_points.end(), { 1.0f,  1.0f,  0.0f});
        m_points.insert(m_points.end(), { 1.0f, -1.0f,  0.0f});
        m_points.insert(m_points.end(), {-1.0f, -1.0f,  0.0f});
    }

    void setColor(float r, float g, float b) {
        m_material.color.x() = r;
        m_material.color.y() = g;
        m_material.color.z() = b;
    }

    void setModel(const orpheus::math::Matrix4x4& model) {
        m_model = model;
    }

    const std::vector<orpheus::math::Vector3>& getPoints() const {
        return m_points;
    }

    const orpheus::math::Vector3& getColor() const {
        return m_material.color;
    }

    const orpheus::math::Matrix4x4& getModel() const {
        return m_model;
    }
};

class EntityAreaLightGroup : public orpheus::Entity {
private:
    std::vector<std::shared_ptr<EntityAreaLight>> m_areaLights;
    orpheus::render::SsboId m_ssbo;

public:
    EntityAreaLightGroup(orpheus::Entity& base, orpheus::Scene& scene) :
        orpheus::Entity(base),
        m_ssbo(scene.createSSBO())
    { }

    orpheus::render::SsboId getSSBO() const {
        return m_ssbo;
    }

    std::uint64_t getCount() const {
        return m_areaLights.size();
    }

    void addAreaLight(const std::shared_ptr<EntityAreaLight>& areaLight) {
        m_areaLights.push_back(areaLight);
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        std::uint32_t maxLights     = 10000;
        std::uint32_t countSize     =               4 * sizeof(std::uint32_t);
        std::uint32_t colorsSize    = maxLights *   4 * sizeof(float);
        std::uint32_t positionsSize = maxLights * 4*4 * sizeof(float);
        std::uint32_t indicesSize   = maxLights * 4*4 * sizeof(std::uint32_t);

        render->setSSBO(m_ssbo);
        render->ssboSetSize(countSize + colorsSize + positionsSize + indicesSize);

        void* buffer = render->ssboMapBuffer();

        std::uint32_t* countPtr        = static_cast<std::uint32_t*>(buffer);
        float*         colorsBuffer    = static_cast<float*>        (static_cast<void*>(static_cast<char*>(buffer) + countSize));
        float*         positionsBuffer = static_cast<float*>        (static_cast<void*>(static_cast<char*>(buffer) + countSize + colorsSize));
        std::uint32_t* indicesBuffer   = static_cast<std::uint32_t*>(static_cast<void*>(static_cast<char*>(buffer) + countSize + colorsSize + positionsSize));

        countPtr[0] = m_areaLights.size();

        std::uint32_t colorsIt = 0;
        std::uint32_t positionsIt = 0;
        std::uint32_t indicesIt = 0;

        for (const auto& areaLight : m_areaLights) {
            auto areaModel = areaLight->getModel();

            std::size_t offset = positionsIt / 4;
            std::size_t count = areaLight->getPoints().size();

            colorsBuffer[colorsIt++] = areaLight->getColor().x();
            colorsBuffer[colorsIt++] = areaLight->getColor().y();
            colorsBuffer[colorsIt++] = areaLight->getColor().z();
            colorsBuffer[colorsIt++] = 1.0f;

            for (const auto& point : areaLight->getPoints()) {                
                orpheus::math::Vector4 pSrc = {point.x(), point.y(), point.z(), 1.0f};
                orpheus::math::Vector4 pDst;
                m_math->mul(pDst, areaModel, pSrc);

                positionsBuffer[positionsIt++] = pDst.x();
                positionsBuffer[positionsIt++] = pDst.y();
                positionsBuffer[positionsIt++] = pDst.z();
                positionsBuffer[positionsIt++] = pDst.w();
            }

            for (std::size_t i = 0; i < count; i++) {
                indicesBuffer[indicesIt++] = offset + i;
                indicesBuffer[indicesIt++] = offset + (i + 1) % count;
                indicesBuffer[indicesIt++] = 0;
                indicesBuffer[indicesIt++] = 0;
            }
        }

        render->ssboUnmapBuffer();
    }
};

class EntityReflectFloor : public orpheus::Entity {
private:
    orpheus::math::Matrix4x4              m_model;
    orpheus::render::material::FlatColor  m_materialColor;
    orpheus::render::material::GGX        m_materialGgx;
    orpheus::math::Vector3                m_position;
    float                                 m_timer;
    std::shared_ptr<EntityAreaLightGroup> m_areaLightGroup;
    std::shared_ptr<EntityAreaLight>      m_areaLight1;
    std::shared_ptr<EntityAreaLight>      m_areaLight2;
    std::shared_ptr<EntityAreaLight>      m_areaLight3;
    std::shared_ptr<EntityAreaLight>      m_areaLight4;

public:
    EntityReflectFloor(orpheus::Entity& base, orpheus::Scene& scene, const std::shared_ptr<EntityAreaLightGroup>& areaLightGroup, const orpheus::math::Vector3& color, float x, float y, float z) :
        orpheus::Entity(base),
        m_position({x, y, z}),
        m_timer(0.0f),
        m_areaLightGroup(areaLightGroup)
    {
        m_materialColor.color = color;

        m_materialGgx.roughness = 0.2f;
        m_materialGgx.lightsBuffer = areaLightGroup->getSSBO();

        m_areaLight1 = scene.addEntity<EntityAreaLight>(0.0f, 0.0f, 0.0f);
        m_areaLight2 = scene.addEntity<EntityAreaLight>(0.0f, 0.0f, 0.0f);
        m_areaLight3 = scene.addEntity<EntityAreaLight>(0.0f, 0.0f, 0.0f);
        m_areaLight4 = scene.addEntity<EntityAreaLight>(0.0f, 0.0f, 0.0f);
        m_areaLight1->setColor(color.x(), color.y(), color.z());
        m_areaLight2->setColor(color.x(), color.y(), color.z());
        m_areaLight3->setColor(color.x(), color.y(), color.z());
        m_areaLight4->setColor(color.x(), color.y(), color.z());
        areaLightGroup->addAreaLight(m_areaLight1);
        areaLightGroup->addAreaLight(m_areaLight2);
        areaLightGroup->addAreaLight(m_areaLight3);
        areaLightGroup->addAreaLight(m_areaLight4);
    }

    virtual void update(float delta) override {
        m_timer += delta;

        m_materialGgx.lightsCount = m_areaLightGroup->getCount();

        m_model = orpheus::math::Matrix4x4{};
        m_math->scale(m_model, 5.0f, 5.0f, 5.0f);
        m_math->translate(m_model, m_position.x(), m_position.y() + 2 * std::abs(std::sin(0.1f * m_timer + m_position.x() * 0.481516 + m_position.z() * 0.2342)), m_position.z());

        orpheus::math::Matrix4x4 model;

        model = m_model;
        m_math->translate(model,  0.0f, 0.0f,  1.01f);
        m_areaLight1->setModel(model);

        model = m_model;
        m_math->translate(model,  0.0f, 0.0f, -1.01f);
        m_math->rotate(model, 0.0f, 1.0f, 0.0f, 3.141592653f * 1.0f);
        m_areaLight2->setModel(model);

        model = m_model;
        m_math->translate(model, -1.005f, 0.0f,  0.0f);
        m_math->rotate(model, 0.0f, 1.0f, 0.0f, -3.141592653f / 2.0f);
        m_areaLight3->setModel(model);

        model = m_model;
        m_math->translate(model,  1.005f, 0.0f,  0.0f);
        m_math->rotate(model, 0.0f, 1.0f, 0.0f, 3.141592653f / 2.0f);
        m_areaLight4->setModel(model);
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        auto model = m_model;
        render->setModel(model);
        render->setMaterial(m_materialColor);
        render->drawCube();

        m_math->translate(model, 0.0f, 1.0f, 0.0f);
        render->setModel(model);
        render->setMaterial(m_materialGgx);
        render->drawPlane();
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

        auto areaLightGroup = addEntity<EntityAreaLightGroup>();

        std::vector<std::shared_ptr<EntityReflectFloor>> floors;
        for (int i = -25; i < 25; i++) {
            for (int j = -25; j < 25; j++) {
                orpheus::math::Vector3 color;
                m_math->hsv2rgb(color, 360.0f * std::fmod(3.0f * ((i + 5) * 10 + (j + 5)) / 100.0f, 1.0f), 0.75f, 0.4f);
                floors.push_back(addEntity<EntityReflectFloor>(areaLightGroup, color, i * 2.01f, 0.0, -j * 2.01f));
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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);
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
        SDL_Init(SDL_INIT_VIDEO);
        m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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

class RenderContextSDLVulkan : public orpheus::interface::IRenderContext {
private:
    SDL_vulkanInstance m_vkInstance;

public:
    RenderContextSDLVulkan(SDL_Window* window) {
        uint32_t extensionCount;
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
        std::vector<const char *> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames.data());

        VkApplicationInfo appInfo = {};
        std::vector<const char*> layerNames;

        VkInstanceCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        info.pApplicationInfo = &appInfo;
        info.enabledLayerCount = layerNames.size();
        info.ppEnabledLayerNames = layerNames.data();
        info.enabledExtensionCount = extensionNames.size();
        info.ppEnabledExtensionNames = extensionNames.data();

        if (vkCreateInstance(&info, nullptr, &m_vkInstance) != VK_SUCCESS) {
            throw std::runtime_error("vulkan initialization failed");
        }
    }

    ~RenderContextSDLVulkan() {
        vkDestroyInstance(m_vkInstance, nullptr);
    }
};

class WindowSDLVulkan : public orpheus::interface::IWindow {
private:
    SDL_Window*   m_window;

public:
    virtual void create(const std::string& title, std::uint32_t width, std::uint32_t height) override {
        SDL_Init(SDL_INIT_VIDEO);
        m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    virtual std::shared_ptr<orpheus::interface::IRenderContext> createContext() override {
        return std::make_shared<RenderContextSDLVulkan>(m_window);
    }

    virtual void destroy() override {
        SDL_DestroyWindow(m_window);
    }

    virtual void swapBuffers() override {
    }
};

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#else
int main() {
#endif
    auto window  = std::make_shared<WindowSDLOpenGL>();
    auto math    = std::make_shared<orpheus::math::cglm::CGLM>();
    auto render  = std::make_shared<orpheus::render::opengl::OpenGL>(math);
    auto input   = std::make_shared<InputSDL>();
    auto physics = nullptr;//std::make_shared<orpheus::physics::physx::PhysX>();
    auto scene   = std::make_shared<TestScene2>(math, input, physics);

    orpheus::Engine(window, render, input, physics, math, scene).run();

    return 0;
}
