#version 430 core

uniform mat4 u_modelViewProjection;

out vec2 v_uv;

void main() {
    v_uv = vec2(((gl_VertexID % 3) + gl_VertexID / 3) / 2, ((gl_VertexID % 3) + gl_VertexID / 3) % 2);
    gl_Position = u_modelViewProjection * vec4(v_uv, -1.0, 1.0);
}
