#version 430 core

uniform sampler2D u_textureFloorColor;
uniform sampler2D u_textureFloorNormal;
uniform sampler2D u_textureFloorRoughness;
uniform vec2 u_resolution;

in vec3 v_normal;
in vec3 v_position;
in vec2 v_uv;
in vec4 v_curRelPosition;
in vec4 v_prevRelPosition;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outPosition;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out float outRoughness;
layout (location = 4) out vec2 outMotionVector;

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
    outMotionVector = u_resolution * ((v_curRelPosition / v_curRelPosition.w) - (v_prevRelPosition / v_prevRelPosition.w)).xy;
}
