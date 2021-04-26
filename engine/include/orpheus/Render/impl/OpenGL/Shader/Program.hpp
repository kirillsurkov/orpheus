#pragma once

#include "orpheus/Vertex.hpp"

#include <GL/glew.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace Orpheus::Render::OpenGLImpl::Shader {
    enum class Type {
        VERTEX,
        FRAGMENT
    };

    class Shader {
    private:
        GLuint m_id;

        void checkShaderStatus(unsigned int shader) {
            int success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (success == GL_TRUE) {
                return;
            }

            std::size_t bufferSize = 2048;
            std::vector<char> buffer(bufferSize);
            int len;

            glGetShaderInfoLog(shader, bufferSize, &len, buffer.data());

            throw std::runtime_error(std::string(buffer.data(), len));
        }

    public:
        Shader(Type type, const std::string& source) {
            auto src = source.c_str();
            switch (type) {
                case Type::VERTEX: {
                    m_id = glCreateShader(GL_VERTEX_SHADER);
                    break;
                }
                case Type::FRAGMENT: {
                    m_id = glCreateShader(GL_FRAGMENT_SHADER);
                    break;
                }
                default: {
                    throw std::runtime_error("Unsupported shader type");
                }
            }

            glShaderSource(m_id, 1, &src, nullptr);
            glCompileShader(m_id);
            checkShaderStatus(m_id);
        }

        ~Shader() {
            glDeleteShader(m_id);
        }

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        GLuint getId() const {
            return m_id;
        }
    };

    class Program {
    private:
        GLuint m_id;
        mutable std::unordered_map<std::string, GLint> m_uniforms;
        std::unordered_map<GLuint, GLuint> m_layouts;

    public:
        Program() {
            m_id = glCreateProgram();
            if (m_id == 0) {
                throw std::runtime_error("glCreateProgram failed");
            }
        }

        ~Program() {
            glDeleteProgram(m_id);
        }

        GLint getUniform(const std::string& name) const {
            auto it = m_uniforms.find(name);
            if (it == m_uniforms.end()) {
                it = m_uniforms.try_emplace(name, glGetUniformLocation(m_id, name.c_str())).first;
            }
            return it->second;
        }

        bool matchLayout(const Vertex::Vertices& vertices) const {
            for (const auto& attrib : vertices.getAttribs()) {
                auto it = m_layouts.find(attrib.getLayout());
                if (it == m_layouts.end() || it->second != attrib.getBuffer().getElements()) {
                    return false;
                }
            }
            return true;
        }

        Program& attach(const Shader& shader) {
            glAttachShader(m_id, shader.getId());

            return *this;
        }

        Program& addLayout(GLuint layout, GLuint elements) {
            m_layouts[layout] = elements;

            return *this;
        }

        Program& link() {
            glLinkProgram(m_id);
            return *this;
        }

        void use() {
            glUseProgram(m_id);
        }
    };
}
