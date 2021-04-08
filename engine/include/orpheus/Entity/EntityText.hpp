#pragma once

#include "orpheus/Entity/Entity.hpp"
#include "orpheus/Command/Render/CommandMaterial.hpp"
#include "orpheus/Command/Material/CommandMatrixProjection.hpp"
#include "orpheus/Command/Material/CommandMatrixView.hpp"
#include "orpheus/Command/Material/CommandMatrixModel.hpp"
#include "orpheus/Command/Material/CommandText.hpp"
#include "orpheus/Material/MaterialText.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace Orpheus::Entity {
    class EntityText : public Entity {
    private:
        float m_x;
        float m_y;
        float m_height;
        std::string m_text;
        Command::Render::CommandMaterial<Material::MaterialText> m_material;

        glm::mat4x4 getTransform() const {
            glm::mat4x4 res(1.0f);
            res = glm::translate(res, glm::vec3(m_x, m_y, 0.0f));
            return res;
        }

    public:
        EntityText(float x, float y, float height, const std::string& text = "") :
            m_x(x),
            m_y(y),
            m_height(height),
            m_text(text)
        {
        }

        virtual void draw(const glm::mat4x4&/* projection*/, const glm::mat4x4&/* view*/, Render::Render& render) override {
            render.postCommand(m_material);
            render.postCommand(Command::Material::CommandMatrixModel(getTransform()));
            render.postCommand(Command::Material::CommandText(m_x, m_y, m_height, m_text));
        }
    };
}
