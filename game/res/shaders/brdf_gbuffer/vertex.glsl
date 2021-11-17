#version 430 core

uniform mat4 u_model;
uniform mat4 u_normalMatrix;
uniform mat4 u_modelViewProjection;
uniform mat4 u_prevModelViewProjection;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 v_normal;
out vec3 v_position;
out vec2 v_uv;
out vec4 v_curRelPosition;
out vec4 v_prevRelPosition;
out mat3 v_TBN;

void main() {
    v_curRelPosition = u_modelViewProjection * vec4(aPosition, 1.0);
    v_prevRelPosition = u_prevModelViewProjection * vec4(aPosition, 1.0);

    gl_Position = v_curRelPosition;
    v_normal = normalize((u_normalMatrix * vec4(aNormal, 0.0)).xyz);
    v_position = (u_model * vec4(aPosition, 1.0)).xyz;

    vec3 T = normalize(vec3(u_model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(u_model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(u_model * vec4(aNormal,    0.0)));
    v_TBN = mat3(T, B, N);

    v_uv = aUV;//0.5 * aPosition.xz + vec2(0.5);
}
