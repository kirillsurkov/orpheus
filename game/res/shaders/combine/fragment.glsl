#version 430 core

uniform sampler2D u_texture1;
uniform sampler2D u_textureDepth1;
uniform sampler2D u_texture2;
uniform sampler2D u_textureDepth2;

in vec2 v_uv;

layout (location = 0) out vec4 outColor;

void main() {
    if (texture2D(u_textureDepth1, v_uv).x < texture2D(u_textureDepth2, v_uv).x) {
        outColor = texture2D(u_texture1, v_uv);
    } else {
        outColor = texture2D(u_texture2, v_uv);
    }
}
