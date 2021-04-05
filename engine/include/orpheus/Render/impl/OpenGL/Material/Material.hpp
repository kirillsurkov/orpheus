#pragma once

#include "orpheus/Dispatcher.hpp"
#include <GL/glew.h>

namespace Orpheus::Render::OpenGLImpl::Material {
    class Material {
    private:
        unsigned int m_vertexShader;
        unsigned int m_fragmentShader;
        CommandDispatcher m_commandDispatcher;

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

    protected:
        unsigned int m_program;

        template<class T, class U>
        void registerMaterialCommand(U&& receiver) {
            m_commandDispatcher.registerCommand<T>(std::forward<U>(receiver));
        }

    public:
        Material(const std::string& vertexSource, const std::string& fragmentSource) :
            m_vertexShader(0),
            m_fragmentShader(0),
            m_program(0)
        {
            auto vertexShaderSrc = vertexSource.c_str();
            m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(m_vertexShader, 1, &vertexShaderSrc, nullptr);
            glCompileShader(m_vertexShader);
            checkShaderStatus(m_vertexShader);

            auto fragmentShaderSrc = fragmentSource.c_str();
            m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(m_fragmentShader, 1, &fragmentShaderSrc, nullptr);
            glCompileShader(m_fragmentShader);
            checkShaderStatus(m_fragmentShader);

            m_program = glCreateProgram();
            glAttachShader(m_program, m_vertexShader);
            glAttachShader(m_program, m_fragmentShader);
            glLinkProgram(m_program);
        }

        ~Material() {
            if (m_vertexShader) {
                if (m_program) {
                    glDetachShader(m_program, m_vertexShader);
                }
                glDeleteShader(m_vertexShader);
            }
            if (m_fragmentShader) {
                if (m_program) {
                    glDetachShader(m_program, m_fragmentShader);
                }
                glDeleteShader(m_fragmentShader);
            }
        }

        void use() {
            glUseProgram(m_program);
        }

        template<class T>
        void postMaterialCommand(T&& command) {
            m_commandDispatcher.dispatchOrThrow(std::forward<T>(command));
        }
    };

    using MaterialPtr = std::shared_ptr<Material>;
}
