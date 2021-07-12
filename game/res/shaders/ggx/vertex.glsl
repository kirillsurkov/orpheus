#version 330 core

uniform mat4 u_model;
uniform mat4 u_normalMatrix;
uniform mat4 u_modelViewProjection;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 v_normal;
out vec3 v_position;

void main() {
    gl_Position = u_modelViewProjection * vec4(aPosition, 1.0);
    v_normal = normalize((u_normalMatrix * vec4(aNormal, 0.0)).xyz);
    v_position = (u_model * vec4(aPosition, 1.0)).xyz;
}
