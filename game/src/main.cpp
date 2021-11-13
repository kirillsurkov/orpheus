#include <SDL.h>

#include <orpheus/Engine.hpp>

#include <math/cglm/CGLM.hpp>
#include <physics/bullet/Bullet.hpp>
#include <render/opengl/OpenGL.hpp>

#include <PerlinNoise.hpp>

#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>

#include <locale>
#include <codecvt>

class EntityText : public orpheus::Entity {
private:
    struct codecvt : std::codecvt<char32_t, char, std::mbstate_t> {
        ~codecvt() {}
    };

    const orpheus::Font& m_font;
    orpheus::math::Matrix4x4 m_model;
    orpheus::render::material::Text m_material;
    std::string m_text;

public:
    EntityText(orpheus::Entity& base, orpheus::Scene& scene, const orpheus::Font& font, float x, float y, const std::string& text) :
        orpheus::Entity(base),
        m_font(font),
        m_text(text)
    {
        m_math->translate(m_model, x, y, 0.0f);
        m_material.font = font.getName();
        m_material.height = 18.0f;
        m_material.descender = font.getDescender();
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);

        float advance = 0.0f;
        std::wstring_convert<codecvt, char32_t> cvt;
        for (std::uint64_t c : cvt.from_bytes(m_text)) {
            m_material.glyph = m_font.getGlyph(c);
            m_material.advance = advance;
            render->setMaterial(m_material);
            render->drawPlane();
            advance += m_material.glyph.getAdvance();
        }
    }

    void setText(const std::string& text) {
        m_text = text;
    }

    const std::string& getText() const {
        return m_text;
    }
};

class EntityPlayer : public orpheus::Entity {
private:
    orpheus::physics::BodyID  m_capsuleId;
    orpheus::math::Vector3    m_dirForward;
    orpheus::math::Vector3    m_dirRight;
    orpheus::math::Vector3    m_position;
    bool m_alive    = true;
    bool m_onGround = false;
    bool m_moved    = false;
    bool m_forward  = false;
    bool m_backward = false;
    bool m_left     = false;
    bool m_right    = false;
    bool m_jump     = false;
    std::uint64_t m_score;
    orpheus::math::Matrix4x4 m_model;
    orpheus::render::material::FlatColor m_material;

public:
    EntityPlayer(orpheus::Entity& base, orpheus::Scene& scene, float x, float y, float z) :
        orpheus::Entity(base),
        m_score(0)
    {
        orpheus::physics::ShapeInfo capsuleShapeInfo;
        capsuleShapeInfo.capsule.radius = 1.0f;
        capsuleShapeInfo.capsule.height = 1.0f;
        m_capsuleId = scene.addBody(orpheus::physics::BodyDescription{
                                        orpheus::physics::Shape::Capsule,
                                        capsuleShapeInfo,
                                        orpheus::physics::Type::Dynamic,
                                        orpheus::math::Vector3{x, y, z},
                                        orpheus::math::Vector3{0.0f, 1.0f, 0.0f},
                                        0.0f
                                    });

        m_material.color = {1.0f, 1.0f, 0.0f};
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
    void jump(bool enable)     { m_jump = enable; }

    const orpheus::math::Vector3& getPosition() const {
        return m_position;
    }

    orpheus::physics::BodyID getBodyID() const {
        return m_capsuleId;
    }

    bool isMoved() {
        return m_moved;
    }

    std::uint64_t getScore() const {
        return m_score;
    }

    void addScore(std::uint64_t score) {
        m_score += score;
    }

    void kill() {
        m_alive = false;
    }

    bool isAlive() const {
        return m_alive;
    }

    void respawn() {
        m_score = 0;
        m_alive = true;
    }

    virtual void update(float delta) override {
        m_onGround = false;

        float legThreshold = -std::cos(M_PI * 1.0f / 3.0f);
        for (const auto& normal : m_physics->getNormals(m_capsuleId)) {
            //std::cout << normal.x() << " " << normal.y() << " " << normal.z() << std::endl;
            if (normal.y() <= legThreshold) {
                m_onGround = true;
                break;
            }
        }

        orpheus::math::Vector4 position = {0.0f, 0.0f, 0.0f, 1.0f};
        m_math->mul(position, m_physics->getState(m_capsuleId).transform, position);
        m_position = {position.x(), position.y(), position.z()};
        m_position.y() += 2.0f;
        //m_position.z() += 5.0f;

        if (m_alive && m_onGround) {
            orpheus::math::Vector3 direction;
            if (m_forward)  m_math->add(direction, direction, m_dirForward);
            if (m_backward) m_math->sub(direction, direction, m_dirForward);
            if (m_left)     m_math->sub(direction, direction, m_dirRight);
            if (m_right)    m_math->add(direction, direction, m_dirRight);

            m_moved = direction.x() || direction.y() || direction.z();

            if (m_moved) {
                m_math->normalize(direction);
                m_math->mul(direction, direction, 10.0f);
            }

            if (m_jump) {
                m_math->add(direction, direction, orpheus::math::Vector3{0.0f, 20.0f, 0.0f});
                m_jump = false;
            }

            orpheus::math::Vector3 velocity = m_physics->getLinearVelocity(m_capsuleId);
            velocity.x()  = (velocity.x() * 0.5 + direction.x());
            velocity.y() += direction.y();
            velocity.z()  = (velocity.z() * 0.5 + direction.z());

            m_physics->setLinearVelocity(m_capsuleId, velocity);
        }

        m_model = m_physics->getState(m_capsuleId).transform;
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        /*render->setModel(m_model);
        render->setMaterial(m_material);
        render->drawCylinder();

        m_math->translate(m_model, 0.0f, 1.0f, 0.0f);
        render->setModel(m_model);
        render->setMaterial(m_material);
        render->drawSphere();

        m_math->translate(m_model, 0.0f, -2.0f, 0.0f);
        render->setModel(m_model);
        render->setMaterial(m_material);
        render->drawSphere();*/
    }
};

class EntityAreaLight : public orpheus::Entity {
public:
    enum class Type {
        Quad,
        Sphere
    };

private:
    Type                                 m_type;
    orpheus::math::Matrix4x4             m_model;
    orpheus::render::material::FlatColor m_material;
    std::vector<orpheus::math::Vector3>  m_points;

public:
    EntityAreaLight(orpheus::Entity& base, orpheus::Scene& scene, Type type, float x, float y, float z) :
        orpheus::Entity(base),
        m_type(type)
    {
        m_math->translate(m_model, x, y + 2.0f, z);
        m_math->scale(m_model, 2.0f, 2.0f, 1.0f);
        m_math->rotate(m_model, 1.0f, 0.0f, 0.0f, 3.141592653f / 2.0f);

        m_material.color = {1.0f, 1.0f, 1.0f};

        switch (type) {
        case Type::Quad: {
            m_points.insert(m_points.end(), {-1.0f,  1.0f,  0.0f});
            m_points.insert(m_points.end(), { 1.0f,  1.0f,  0.0f});
            m_points.insert(m_points.end(), { 1.0f, -1.0f,  0.0f});
            m_points.insert(m_points.end(), {-1.0f, -1.0f,  0.0f});
            break;
        }
        case Type::Sphere: {
            m_points.insert(m_points.end(), {0.0f, 0.0f, 0.0f});
            m_points.insert(m_points.end(), {   x, 0.0f, 0.0f});
            m_points.insert(m_points.end(), {0.0f, 0.0f, 0.0f});
            m_points.insert(m_points.end(), {0.0f, 0.0f, 0.0f});
        }
        }
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

    Type getType() const {
        return m_type;
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
        std::uint32_t typesSize     = maxLights *   4 * sizeof(std::uint32_t);
        std::uint32_t colorsSize    = maxLights *   4 * sizeof(float);
        std::uint32_t positionsSize = maxLights * 4*4 * sizeof(float);

        render->setSSBO(m_ssbo);
        render->ssboSetSize(countSize + colorsSize + positionsSize + typesSize);

        void* buffer = render->ssboMapBuffer();

        std::uint32_t* countPtr        = static_cast<std::uint32_t*>(buffer);
        std::uint32_t* typesBuffer     = static_cast<std::uint32_t*>(static_cast<void*>(static_cast<char*>(buffer) + countSize));
        float*         colorsBuffer    = static_cast<float*>        (static_cast<void*>(static_cast<char*>(buffer) + countSize + typesSize));
        float*         positionsBuffer = static_cast<float*>        (static_cast<void*>(static_cast<char*>(buffer) + countSize + typesSize + colorsSize));

        countPtr[0] = m_areaLights.size();

        std::uint32_t colorsIt = 0;
        std::uint32_t positionsIt = 0;
        std::uint32_t typesIt = 0;

        for (const auto& areaLight : m_areaLights) {
            auto areaModel = areaLight->getModel();

            switch (areaLight->getType()) {
            case EntityAreaLight::Type::Quad:   typesBuffer[typesIt++] = 0; break;
            case EntityAreaLight::Type::Sphere: typesBuffer[typesIt++] = 1; break;
            }
            typesBuffer[typesIt++] = 0;
            typesBuffer[typesIt++] = 0;
            typesBuffer[typesIt++] = 0;

            colorsBuffer[colorsIt++] = areaLight->getColor().x();
            colorsBuffer[colorsIt++] = areaLight->getColor().y();
            colorsBuffer[colorsIt++] = areaLight->getColor().z();
            colorsBuffer[colorsIt++] = 1.0f;

            const auto& points = areaLight->getPoints();
            for (std::uint32_t i = 0; i < points.size(); i++) {
                const auto& point = points[i];
                orpheus::math::Vector4 pSrc = {point.x(), point.y(), point.z(), 1.0f};
                orpheus::math::Vector4 pDst = pSrc;

                if (areaLight->getType() != EntityAreaLight::Type::Sphere || i == 0) {
                    m_math->mul(pDst, areaModel, pSrc);
                }

                positionsBuffer[positionsIt++] = pDst.x();
                positionsBuffer[positionsIt++] = pDst.y();
                positionsBuffer[positionsIt++] = pDst.z();
                positionsBuffer[positionsIt++] = pDst.w();
            }
        }

        render->ssboUnmapBuffer();
    }
};

class EntityReflectFloor : public orpheus::Entity {
private:
    const siv::PerlinNoise                m_perlin;
    std::shared_ptr<EntityPlayer>         m_player;
    orpheus::math::Matrix4x4              m_model;
    orpheus::math::Matrix4x4              m_prevModel;
    orpheus::render::material::FlatColor  m_materialColor;
    orpheus::render::material::GGX        m_materialGgx;
    orpheus::math::Vector3                m_scale;
    orpheus::math::Vector3                m_positionInitial;
    orpheus::math::Vector3                m_positionActual;
    orpheus::math::Vector2                m_noisePosition;
    float                                 m_timer;
    orpheus::physics::BodyID              m_cubeId;
    std::shared_ptr<EntityAreaLight>      m_areaLight1;
    std::shared_ptr<EntityAreaLight>      m_areaLight2;
    std::shared_ptr<EntityAreaLight>      m_areaLight3;
    std::shared_ptr<EntityAreaLight>      m_areaLight4;
    bool                                  m_hueDirectionForward;
    float                                 m_hueOffset;

    void updateActualPosition(float delta) {
        m_positionActual = m_positionInitial;

        orpheus::math::Vector3 playerPos = m_player->getPosition();
        playerPos.y() = 0.0f;
        m_math->normalize(playerPos);

        if (m_player->isMoved()) {
            m_noisePosition.x() += delta * playerPos.x();
            m_noisePosition.y() += delta * playerPos.z();
        }

        float noiseScale = 0.1f;
        float noiseX = m_positionInitial.x() * noiseScale - m_noisePosition.x();
        float noiseY = m_positionInitial.z() * noiseScale - m_noisePosition.y();

        m_positionActual.x() *= m_scale.x();
        m_positionActual.y() += m_scale.y() * 2.0f * m_perlin.noise3D(1.0f + noiseX, 1.0f + noiseY, 1.0f);
        m_positionActual.z() *= m_scale.z();
    }

public:
    EntityReflectFloor(orpheus::Entity& base, orpheus::Scene& scene,
                       const siv::PerlinNoise& perlin,
                       const std::shared_ptr<EntityPlayer>& player,
                       const std::shared_ptr<EntityAreaLightGroup>& areaLightGroup,
                       float hueOffset, float x, float y, float z) :
        orpheus::Entity(base),
        m_perlin(perlin),
        m_player(player),
        m_scale({5.0f, 5.0f, 5.0f}),
        m_positionInitial({x, y, z}),
        m_noisePosition({0.0f, 0.0f}),
        m_timer(0.0f),
        m_hueDirectionForward(true),
        m_hueOffset(hueOffset)
    {
        m_materialGgx.roughness = 0.2f;
        m_materialGgx.lightsBuffer = areaLightGroup->getSSBO();

        orpheus::physics::ShapeInfo shapeInfo;
        shapeInfo.cube.scaleX = m_scale.x();
        shapeInfo.cube.scaleY = m_scale.y();
        shapeInfo.cube.scaleZ = m_scale.z();
        m_cubeId = scene.addBody(orpheus::physics::BodyDescription{
                                     orpheus::physics::Shape::Cube,
                                     shapeInfo,
                                     orpheus::physics::Type::Kinematic,
                                     orpheus::math::Vector3{x * shapeInfo.cube.scaleX, y * shapeInfo.cube.scaleY, z * shapeInfo.cube.scaleZ}
                                 });

        m_areaLight1 = scene.addEntity<EntityAreaLight>(EntityAreaLight::Type::Quad, 0.0f, 0.0f, 0.0f);
        m_areaLight2 = scene.addEntity<EntityAreaLight>(EntityAreaLight::Type::Quad, 0.0f, 0.0f, 0.0f);
        m_areaLight3 = scene.addEntity<EntityAreaLight>(EntityAreaLight::Type::Quad, 0.0f, 0.0f, 0.0f);
        m_areaLight4 = scene.addEntity<EntityAreaLight>(EntityAreaLight::Type::Quad, 0.0f, 0.0f, 0.0f);
        areaLightGroup->addAreaLight(m_areaLight1);
        areaLightGroup->addAreaLight(m_areaLight2);
        areaLightGroup->addAreaLight(m_areaLight3);
        areaLightGroup->addAreaLight(m_areaLight4);

        updateActualPosition(0.0f);
    }

    const orpheus::math::Vector3& getActualPosition() const {
        return m_positionActual;
    }

    const orpheus::math::Vector3& getScale() const {
        return m_scale;
    }

    virtual void update(float delta) override {
        m_hueOffset += delta * 0.25f;
        if (m_hueOffset >= 1.0f) {
            m_hueOffset = std::fmod(m_hueOffset, 1.0f);
            m_hueDirectionForward = !m_hueDirectionForward;
        }

        orpheus::math::Vector3 color;
        m_math->hsv2rgb(color, 60.0f + 180.0f * (m_hueDirectionForward ? m_hueOffset : (1.0f - m_hueOffset)), 0.75f, 1.0f);
        m_materialColor.color = color;
        m_areaLight1->setColor(color.x(), color.y(), color.z());
        m_areaLight2->setColor(color.x(), color.y(), color.z());
        m_areaLight3->setColor(color.x(), color.y(), color.z());
        m_areaLight4->setColor(color.x(), color.y(), color.z());

        m_prevModel = m_model;

        m_timer += delta;

        m_model = m_physics->getState(m_cubeId).transform;
        m_math->scale(m_model, m_scale.x(), m_scale.y(), m_scale.z());

        updateActualPosition(delta);

        m_physics->setPosition(m_cubeId, m_positionActual);

        orpheus::math::Matrix4x4 model;

        model = m_model;
        m_math->translate(model,  0.0f, 0.0f,  1.0f);
        m_areaLight1->setModel(model);

        model = m_model;
        m_math->translate(model,  0.0f, 0.0f, -1.0f);
        m_math->rotate(model, 0.0f, 1.0f, 0.0f, 3.141592653f * 1.0f);
        m_areaLight2->setModel(model);

        model = m_model;
        m_math->translate(model, -1.0f, 0.0f,  0.0f);
        m_math->rotate(model, 0.0f, 1.0f, 0.0f, -3.141592653f / 2.0f);
        m_areaLight3->setModel(model);

        model = m_model;
        m_math->translate(model,  1.0f, 0.0f,  0.0f);
        m_math->rotate(model, 0.0f, 1.0f, 0.0f, 3.141592653f / 2.0f);
        m_areaLight4->setModel(model);
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        auto model = m_model;
        auto prevModel = m_prevModel;
        render->setModel(model);
        render->setPrevModel(prevModel);
        render->setMaterial(m_materialColor);
        render->drawCube();

        m_math->translate(model, 0.0f, 1.0f, 0.0f);
        m_math->translate(prevModel, 0.0f, 1.0f, 0.0f);
        render->setModel(model);
        render->setPrevModel(prevModel);
        render->setMaterial(m_materialGgx);
        render->drawPlane();
    }
};

class EntityBall : public orpheus::Entity {
private:
    orpheus::math::Matrix4x4              m_model;
    bool                                  m_hueDirectionForward;
    float                                 m_hueOffset;
    float                                 m_radius;
    orpheus::render::material::FlatColor  m_material;
    orpheus::math::Vector3                m_position;
    orpheus::physics::BodyID              m_sphereId;
    std::shared_ptr<EntityAreaLight>      m_areaLight;
    const std::shared_ptr<EntityPlayer>&  m_player;

public:
    EntityBall(orpheus::Entity& base, orpheus::Scene& scene,
               const std::shared_ptr<EntityPlayer>& player,
               const std::shared_ptr<EntityAreaLightGroup>& areaLightGroup,
               float hueOffset, float radius, float x, float y, float z) :
        orpheus::Entity(base),
        m_hueDirectionForward(true),
        m_hueOffset(hueOffset),
        m_radius(radius),
        m_position({x, y, z}),
        m_player(player)
    {
        orpheus::physics::ShapeInfo shapeInfo;
        shapeInfo.sphere.radius = radius;
        m_sphereId = scene.addBody(orpheus::physics::BodyDescription{
                                       orpheus::physics::Shape::Sphere,
                                       shapeInfo,
                                       orpheus::physics::Type::Dynamic,
                                       orpheus::math::Vector3{x, y, z}
                                  });

        m_areaLight = scene.addEntity<EntityAreaLight>(EntityAreaLight::Type::Sphere, m_radius, 0.0f, 0.0f);
        areaLightGroup->addAreaLight(m_areaLight);
    }

    virtual void update(float delta) override {
        m_hueOffset += delta * 0.25f;
        if (m_hueOffset >= 1.0f) {
            m_hueOffset = std::fmod(m_hueOffset, 1.0f);
            m_hueDirectionForward = !m_hueDirectionForward;
        }

        orpheus::math::Vector3 color;
        m_math->hsv2rgb(color, 30.0f * (m_hueDirectionForward ? m_hueOffset : (1.0f - m_hueOffset)), 0.85f, 1.0f);
        m_material.color = color;
        m_areaLight->setColor(color.x(), color.y(), color.z());

        m_model = m_physics->getState(m_sphereId).transform;
        m_math->scale(m_model, m_radius, m_radius, m_radius);
        m_areaLight->setModel(m_model);

        orpheus::physics::BodyID playerBodyId = m_player->getBodyID();
        for (orpheus::physics::BodyID collision : m_physics->getCollisions(m_sphereId)) {
            if (collision == playerBodyId) {
                m_player->kill();
                break;
            }
        }
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->setMaterial(m_material);
        render->drawSphere();
    }
};

class EntityReward : public orpheus::Entity {
private:
    orpheus::math::Matrix4x4              m_model;
    bool                                  m_hueDirectionForward;
    float                                 m_hueOffset;
    float                                 m_radius;
    orpheus::render::material::FlatColor  m_material;
    orpheus::physics::BodyID              m_sphereId;
    std::shared_ptr<EntityAreaLight>      m_areaLight;
    std::shared_ptr<EntityPlayer>         m_player;
    float                                 m_timer;
    int                                                     m_currentFloorId;
    const std::vector<std::shared_ptr<EntityReflectFloor>>& m_floors;

    void respawn() {
        m_currentFloorId = rand() % m_floors.size();
    }

public:
    EntityReward(orpheus::Entity& base, orpheus::Scene& scene,
                 const std::shared_ptr<EntityPlayer>& player,
                 const std::vector<std::shared_ptr<EntityReflectFloor>>& floors,
                 const std::shared_ptr<EntityAreaLightGroup>& areaLightGroup,
                 float radius) :
        orpheus::Entity(base),
        m_hueDirectionForward(true),
        m_hueOffset(0.0f),
        m_radius(radius),
        m_player(player),
        m_timer(0.0f),
        m_floors(floors)
    {
        orpheus::physics::ShapeInfo shapeInfo;
        shapeInfo.sphere.radius = radius;
        m_sphereId = scene.addBody(orpheus::physics::BodyDescription{
                                       orpheus::physics::Shape::Sphere,
                                       shapeInfo,
                                       orpheus::physics::Type::Static,
                                       orpheus::math::Vector3{0.0f, 0.0f, 0.0f}
                                  });

        m_areaLight = scene.addEntity<EntityAreaLight>(EntityAreaLight::Type::Sphere, m_radius, 0.0f, 0.0f);
        areaLightGroup->addAreaLight(m_areaLight);

        respawn();
    }

    virtual void update(float delta) override {
        m_hueOffset += delta * 0.25f;
        if (m_hueOffset >= 1.0f) {
            m_hueOffset = std::fmod(m_hueOffset, 1.0f);
            m_hueDirectionForward = !m_hueDirectionForward;
        }

        orpheus::math::Vector3 color;
        m_math->hsv2rgb(color, 110.0f + 20.0f * (m_hueDirectionForward ? m_hueOffset : (1.0f - m_hueOffset)), 0.9f, 1.0f);
        m_material.color = color;
        m_areaLight->setColor(color.x(), color.y(), color.z());

        m_timer = std::fmod(m_timer + delta, 10.0f);

        m_model = m_physics->getState(m_sphereId).transform;
        m_math->scale(m_model, m_radius, m_radius, m_radius);
        m_areaLight->setModel(m_model);

        orpheus::math::Vector3 curPos = m_physics->getPosition(m_sphereId);

        orpheus::physics::BodyID playerBodyId = m_player->getBodyID();
        for (orpheus::physics::BodyID collision : m_physics->getCollisions(m_sphereId)) {
            if (collision == playerBodyId) {
                respawn();

                orpheus::math::Vector3 playerPos = m_player->getPosition();
                m_math->sub(playerPos, playerPos, curPos);
                float len = playerPos.x() * playerPos.x() + playerPos.y() * playerPos.y() + playerPos.z() * playerPos.z();

                if (len <= 50.0f) {
                    m_player->addScore(100);
                }

                break;
            }
        }

        float pathLength = 2.0f;

        orpheus::math::Vector3 position = m_floors[m_currentFloorId]->getActualPosition();
        position.y() += m_floors[m_currentFloorId]->getScale().y() + m_radius + pathLength * (1.0f + std::sin(m_timer * M_PI));
        m_physics->setPosition(m_sphereId, position);
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        render->setModel(m_model);
        render->setMaterial(m_material);
        render->drawSphere();
    }
};

class EntityWall : public orpheus::Entity {
private:
    orpheus::math::Matrix4x4              m_model;
    orpheus::render::material::FlatColor  m_material;
    orpheus::math::Vector3                m_position;
    orpheus::math::Vector3                m_scale;
    orpheus::physics::BodyID              m_cubeId;

public:
    EntityWall(orpheus::Entity& base, orpheus::Scene& scene, float x, float y, float z, float scaleX, float scaleY, float scaleZ) :
        orpheus::Entity(base),
        m_position({x, y, z}),
        m_scale({scaleX, scaleY, scaleZ})
    {
        m_material.color = {0.0f, 0.5f, 0.0f};

        orpheus::physics::ShapeInfo shapeInfo;
        shapeInfo.cube.scaleX = scaleX;
        shapeInfo.cube.scaleY = scaleY;
        shapeInfo.cube.scaleZ = scaleZ;
        m_cubeId = scene.addBody(orpheus::physics::BodyDescription{
                                     orpheus::physics::Shape::Cube,
                                     shapeInfo,
                                     orpheus::physics::Type::Kinematic,
                                     orpheus::math::Vector3{x, y, z}
                                 });
    }

    virtual void update(float delta) override {
        m_model = m_physics->getState(m_cubeId).transform;
        m_math->scale(m_model, m_scale.x(), m_scale.y(), m_scale.z());
    }

    virtual void draw(const std::shared_ptr<orpheus::interface::IRender>& render) override {
        /*render->setModel(m_model);
        render->setMaterial(m_material);
        render->drawCube();*/
    }
};

class TestScene : public orpheus::Scene {
private:
    siv::PerlinNoise                                 m_perlin;
    std::shared_ptr<EntityPlayer>                    m_player;
    std::vector<std::shared_ptr<EntityReflectFloor>> m_floors;
    orpheus::Font                                    m_font;

    std::deque<float> m_deltaBuffer;

    std::shared_ptr<EntityText> m_labelScore;
    std::shared_ptr<EntityText> m_labelFPS;
    std::shared_ptr<EntityText> m_labelDead;

public:
    TestScene(const std::shared_ptr<orpheus::interface::IMath>& math,
              const std::shared_ptr<orpheus::interface::IInput>& input,
              const std::shared_ptr<orpheus::interface::IPhysics>& physics) :
        orpheus::Scene(math, input, physics),
        m_font("noto-sans-mono")
    {
        m_camera.setPosition(orpheus::math::Vector3{0.0f, 30.0f, 0.0f});

        m_labelScore = addEntity<EntityText>(m_font, 10.0f, 10.0f, "");
        m_labelFPS = addEntity<EntityText>(m_font, 10.0f, 10.0f + 20.0f, "");
        m_labelDead = addEntity<EntityText>(m_font, 10.0f, 10.0f + 20.0f * 2, "");

        m_player = addEntity<EntityPlayer>(-4.5f, 15.0f, -20.0f);

        auto areaLightGroup = addEntity<EntityAreaLightGroup>();

        for (int i = -10; i < 10; i++) {
            for (int j = -10; j < 10; j++) {
                float x = (i + 0.5f) * 2.01f;
                float z = (j + 0.5f) * 2.01f;
                float hueOffset = rand() / static_cast<double>(RAND_MAX);

                m_floors.push_back(addEntity<EntityReflectFloor>(m_perlin, m_player, areaLightGroup, hueOffset, x, 0.0, z));
            }
        }

        addEntity<EntityReward>(m_player, m_floors, areaLightGroup, 2.0f);

        for (std::uint32_t i = 0; i < 20; i++) {
            std::uint32_t index = rand() % m_floors.size();
            float x = m_floors[index]->getActualPosition().x();
            float z = m_floors[index]->getActualPosition().z();
            float hueOffset = rand() / static_cast<double>(RAND_MAX);
            float radius = 3.0f + 3.0f * (rand() / static_cast<double>(RAND_MAX));

            addEntity<EntityBall>(m_player, areaLightGroup, hueOffset, radius, x, 50.0f, z);
        }

        float wallWidth = 5.0f * 2.01f * 10.0f;
        float wallHeight = 100.0f;
        addEntity<EntityWall>( wallWidth + 1.0f, 0.5f * wallHeight,              0.0f,      1.0f, wallHeight, wallWidth);
        addEntity<EntityWall>(-wallWidth - 1.0f, 0.5f * wallHeight,              0.0f,      1.0f, wallHeight, wallWidth);
        addEntity<EntityWall>(             0.0f, 0.5f * wallHeight,  wallWidth + 1.0f, wallWidth, wallHeight,      1.0f);
        addEntity<EntityWall>(             0.0f, 0.5f * wallHeight, -wallWidth - 1.0f, wallWidth, wallHeight,      1.0f);
        addEntity<EntityWall>(             0.0f, 1.0f + wallHeight,              0.0f, wallWidth,       1.0f, wallWidth);

        /*bindKey(orpheus::input::Key::W, [&](orpheus::input::State state) { m_camera.flyForward(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::S, [&](orpheus::input::State state) { m_camera.flyBackward(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::A, [&](orpheus::input::State state) { m_camera.flyLeft(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::D, [&](orpheus::input::State state) { m_camera.flyRight(state == orpheus::input::State::Down); });*/
        bindKey(orpheus::input::Key::W, [&](orpheus::input::State state) { m_player->forward(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::S, [&](orpheus::input::State state) { m_player->backward(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::A, [&](orpheus::input::State state) { m_player->left(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::D, [&](orpheus::input::State state) { m_player->right(state == orpheus::input::State::Down); });
        bindKey(orpheus::input::Key::SPACE, [&](orpheus::input::State state) { m_player->jump(state == orpheus::input::State::Down); });
    }

    virtual void userUpdate(float delta) override {
        m_deltaBuffer.push_back(delta);
        if (m_deltaBuffer.size() > 20) {
            m_deltaBuffer.pop_front();
        }
        float totalDelta = 0.0f;
        for (float f : m_deltaBuffer) {
            totalDelta += f;
        }

        m_labelFPS->setText("FPS: " + std::to_string(m_deltaBuffer.size() / totalDelta));
        m_labelScore->setText("SCORE: " + std::to_string(m_player->getScore()));

        if (m_player->isAlive()) {
            m_labelDead->setText("");
        } else {
            if (m_labelDead->getText().size() == 0) {
                m_labelDead->setText("YOU DIED! PRESS 'R' TO RESTART");
                bindKey(orpheus::input::Key::R, [&](orpheus::input::State state) {
                    if (state == orpheus::input::State::Down) {
                        m_player->respawn();
                        unbindKey(orpheus::input::Key::R);
                    }
                });
            }
        }

        m_player->setDirectionVectors(m_camera.getForward(), m_camera.getRight());

        auto camera = m_camera.getPosition();
        auto player = m_player->getPosition();

        orpheus::math::Vector3 diff;
        m_math->sub(diff, player, camera);
        m_math->mul(diff, diff, 0.8f);
        m_math->add(diff, camera, diff);

        m_camera.setPosition(diff);
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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
    std::uint32_t m_width;
    std::uint32_t m_height;
    SDL_Window*   m_window;

public:
    virtual void create(const std::string& title) override {
        SDL_Init(SDL_INIT_VIDEO);

        std::vector<SDL_DisplayMode> modes;
        {
            std::uint32_t modeIndex = 0;
            SDL_DisplayMode mode;
            while (SDL_GetDisplayMode(0, modeIndex++, &mode) == 0) {
                modes.push_back(mode);
            }
        }

        std::cout << "Select a resolution: " << std::endl;
        for (std::uint32_t i = 0; i < modes.size(); i++) {
            const auto& mode = modes[i];
            std::cout << " " << (i + 1) << ": " << mode.w << "x" << mode.h << "@" << mode.refresh_rate << std::endl;
        }

        std::uint32_t modeIndex;
        //std::cin >> modeIndex;
        modeIndex = 10;
        modeIndex--;

        if (modeIndex >= modes.size()) {
            throw std::runtime_error("Invalid option");
        }

        m_width = modes[modeIndex].w;
        m_height = modes[modeIndex].h;
        m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_OPENGL);

        //SDL_SetWindowFullscreen(m_window, SDL_TRUE);
        SDL_SetWindowDisplayMode(m_window, &modes[modeIndex]);

        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    virtual std::shared_ptr<orpheus::interface::IRenderContext> createContext() override {
        return std::make_shared<RenderContextSDLOpenGL>(m_window);
    }

    virtual void destroy() override {
        SDL_DestroyWindow(m_window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        SDL_Quit();
    }

    virtual void swapBuffers() override {
        SDL_GL_SwapWindow(m_window);
    }

    virtual std::uint32_t getWidth() const override {
        return m_width;
    }

    virtual std::uint32_t getHeight() const override {
        return m_height;
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
    auto physics = std::make_shared<orpheus::physics::bullet::Bullet>();
    auto scene   = std::make_shared<TestScene>(math, input, physics);

    orpheus::Engine(window, render, input, physics, math, scene).run();

    return 0;
}
