#version 330 core

uniform sampler2D u_texture;
uniform sampler2D u_texture1;
uniform sampler2D u_textureDepth1;
uniform sampler2D u_texture2;
uniform sampler2D u_textureDepth2;
uniform vec2 u_direction;
uniform vec2 u_resolution;

in vec2 v_uv;

layout (location = 0) out vec4 outColor;

void main() {
    float depth1 = texture2D(u_textureDepth1, v_uv).x;
    float depth2 = texture2D(u_textureDepth2, v_uv).x;

    if (depth1 < depth2) {
        outColor = texture2D(u_texture1, v_uv);
    } else {
        outColor = texture2D(u_texture2, v_uv);
    }

    /*vec2 off1 = vec2(1.3333333333333333) * u_direction / u_resolution;
    outColor = texture2D(u_texture, v_uv) * 0.29411764705882354;
    outColor += texture2D(u_texture, v_uv + off1) * 0.35294117647058826;
    outColor += texture2D(u_texture, v_uv - off1) * 0.35294117647058826;*/
}
