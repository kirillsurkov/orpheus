#version 330 core

uniform mat4 u_model;
uniform mat4 u_normalMatrix;
uniform mat4 u_modelViewProjection;
uniform mat4 u_prevModelViewProjection;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 v_normal;
out vec3 v_position;
out vec2 v_uv;
out vec4 v_curRelPosition;
out vec4 v_prevRelPosition;

void main() {
    v_curRelPosition = u_modelViewProjection * vec4(aPosition, 1.0);
    v_prevRelPosition = u_prevModelViewProjection * vec4(aPosition, 1.0);

    gl_Position = v_curRelPosition;
    v_normal = normalize((u_normalMatrix * vec4(aNormal, 0.0)).xyz);
    v_position = (u_model * vec4(aPosition, 1.0)).xyz;

    v_uv = 0.5 * aPosition.xz + vec2(0.5);
}
