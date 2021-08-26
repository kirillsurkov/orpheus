#version 460 core

uniform sampler2D u_textureFloorColor;
uniform sampler2D u_textureFloorNormal;
uniform sampler2D u_textureFloorRoughness;

in vec3 v_normal;
in vec3 v_position;
in vec2 v_uv;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outPosition;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out float outRoughness;

vec3 floorGetColor() {
    vec3 c = texture2D(u_textureFloorColor, v_uv).rgb;
    return vec3((c.x + c.y + c.z) / 3.0);
}

vec3 floorGetNormal() {
    return texture2D(u_textureFloorNormal, v_uv).xzy * 2.0 - 1.0;
}

float floorGetRoughness() {
    return texture2D(u_textureFloorRoughness, v_uv).r;
}

void main() {
    outColor = floorGetColor();
    outPosition = v_position;
    outNormal = floorGetNormal();
    outRoughness = floorGetRoughness();
}
