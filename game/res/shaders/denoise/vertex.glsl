#version 430 core

out vec2 v_uv;

void main() {
    v_uv = vec2(((gl_VertexID % 3) + gl_VertexID / 3) / 2, ((gl_VertexID % 3) + gl_VertexID / 3) % 2);
    gl_Position = vec4(2.0 * v_uv.x - 1.0, 2.0 * v_uv.y - 1.0, 0.0, 1.0);
}
