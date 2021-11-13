#version 430 core

uniform sampler2D u_texture;
uniform vec2      u_textureGlyphOffset;
uniform vec2      u_textureGlyphSize;
uniform vec2      u_textureSize;
uniform vec4      u_textColor;
uniform vec4      u_outlineColor;
uniform float     u_outline;

in vec2 v_uv;

out vec4 color;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float antialias(float aa, float value) {
    return smoothstep(-0.75, 0.75, aa * (value - 0.5));
}

void main() {
    vec2 uv = u_textureGlyphOffset + v_uv * u_textureGlyphSize;
    vec2 dPx = vec2(dFdx(uv.x), dFdy(uv.y)) * u_textureGlyphSize * u_textureSize;
    float aa = 1.4142135623730950488 * inversesqrt(dPx.x * dPx.x + dPx.y * dPx.y);
    vec4 mtsdf = texture(u_texture, uv);
    float alpha = antialias(aa, median(mtsdf.r, mtsdf.g, mtsdf.b));
    float outline = antialias(aa, mtsdf.a + u_outline);
    vec4 glyphColor = vec4(u_textColor.xyz, 1.0) * u_textColor.a * alpha;
    vec4 outlineColor = vec4(u_outlineColor.xyz, 1.0) * u_outlineColor.a * outline * (1.0 - glyphColor.a);
    color = glyphColor + outlineColor;
    //color += vec4(uv.x, uv.y, 0.0, 1.0);
}
