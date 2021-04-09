#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Command/Render/CommandText.hpp"
#include "orpheus/Command/Render/CommandMaterial.hpp"
#include "orpheus/Command/Material/CommandMatrixProjection.hpp"
#include "orpheus/Command/Material/CommandMatrixView.hpp"
#include "orpheus/Command/Material/CommandMatrixModel.hpp"
#include "orpheus/Material/MaterialText.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace Orpheus::Entity {
    class EntityText : public Entity {
    private:
        float m_x;
        float m_y;
        float m_height;
        std::string m_text;
        std::string m_font;
        //float m_size;
        Command::Render::CommandMaterial<Material::MaterialText> m_material;

        glm::mat4x4 getTransform() const {
            glm::mat4x4 res(1.0f);
            /*res = glm::translate(res, glm::vec3(m_x, m_y, 0.0f));
            res = glm::scale(res, glm::vec3(0.1f, 0.1f, 1.0f));*/
            return res;
        }

    public:
        EntityText(float x, float y, float height, const std::string& text = "") :
            m_x(x),
            m_y(y),
            m_height(height),
            m_text(text),
            m_font("ubuntu-mono")
        {
        }

        virtual void draw(const glm::mat4x4& projection, const glm::mat4x4& view, Render::Render& render) override {
            render.postCommand(m_material);
            render.postCommand(Command::Material::CommandMatrixProjection(projection));
            render.postCommand(Command::Material::CommandMatrixView(view));
            render.postCommand(Command::Material::CommandMatrixModel(getTransform()));
            render.postCommand(Command::Render::CommandText(m_x, m_y, m_height, m_text, m_font));
        }

        virtual void update(float delta) override {
            //static float m_timer = 0.0f;
            //m_size = 0.1f;//std::abs(std::sin(m_timer += delta * 0.5f)) * 5;
        }
    };
}
