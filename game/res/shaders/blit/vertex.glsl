#version 330 core

out vec2 v_uv;

void main() {
    v_uv = vec2(((gl_VertexID % 3) + gl_VertexID / 3) / 2, ((gl_VertexID % 3) + gl_VertexID / 3) % 2);
    gl_Position = vec4(v_uv * 2.0 - 1.0, 0.0, 1.0);
}
