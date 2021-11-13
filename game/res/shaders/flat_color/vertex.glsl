#version 430 core

uniform mat4 u_modelViewProjection;

layout(location = 0) in vec3 aPosition;

void main() {
    gl_Position = u_modelViewProjection * vec4(aPosition, 1.0);
}
