#version 330 core

uniform sampler2D u_texture;
uniform vec2 u_direction;
uniform vec2 u_resolution;

in vec2 v_uv;

layout (location = 0) out vec4 outColor;

void main() {
    vec2 off1 = vec2(1.3333333333333333) * u_direction / u_resolution;
    outColor = texture2D(u_texture, v_uv) * 0.29411764705882354;
    outColor += texture2D(u_texture, v_uv + off1) * 0.35294117647058826;
    outColor += texture2D(u_texture, v_uv - off1) * 0.35294117647058826;
}
