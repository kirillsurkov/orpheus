#version 330 core

uniform sampler2D u_color;
uniform sampler2D u_noise;
uniform sampler2D u_lastDenoise;

in vec2 v_uv;

layout (location = 0) out vec4 outColor;

vec3 encodePalYuv(vec3 rgb) {
    rgb = pow(rgb, vec3(2.0));
    return vec3(
        dot(rgb, vec3(0.299, 0.587, 0.114)),
        dot(rgb, vec3(-0.14713, -0.28886, 0.436)),
        dot(rgb, vec3(0.615, -0.51499, -0.10001)));
}

vec3 decodePalYuv(vec3 yuv) {
    vec3 rgb = vec3(
        dot(yuv, vec3(1., 0., 1.13983)),
        dot(yuv, vec3(1., -0.39465, -0.58060)),
        dot(yuv, vec3(1., 2.03211, 0.)));
    return pow(rgb, vec3(1.0 / 2.0));
}

void main() {
    vec4 lastColor = texture2D(u_lastDenoise, v_uv);
    float mixRate = min(lastColor.a, 0.5);

    vec2 offset = 1.0 / vec2(800.0, 600.0);
    vec3 in0 = texture2D(u_noise, v_uv).rgb;

    vec3 denoised = lastColor.rgb;
    denoised = mix(denoised * denoised, in0 * in0, mixRate);
    denoised = sqrt(denoised);

    denoised = encodePalYuv(denoised);
    in0 = encodePalYuv(in0);
    vec3 in1 = encodePalYuv(texture2D(u_noise, v_uv + vec2( offset.x, 0.0)).rgb);
    vec3 in2 = encodePalYuv(texture2D(u_noise, v_uv + vec2(-offset.x, 0.0)).rgb);
    vec3 in3 = encodePalYuv(texture2D(u_noise, v_uv + vec2( 0.0,  offset.y)).rgb);
    vec3 in4 = encodePalYuv(texture2D(u_noise, v_uv + vec2( 0.0, -offset.y)).rgb);
    vec3 in5 = encodePalYuv(texture2D(u_noise, v_uv + vec2( offset.x,  offset.y)).rgb);
    vec3 in6 = encodePalYuv(texture2D(u_noise, v_uv + vec2(-offset.x,  offset.y)).rgb);
    vec3 in7 = encodePalYuv(texture2D(u_noise, v_uv + vec2( offset.x, -offset.y)).rgb);
    vec3 in8 = encodePalYuv(texture2D(u_noise, v_uv + vec2(-offset.x, -offset.y)).rgb);

    vec3 minColor = min(min(min(in0, in1), min(in2, in3)), in4);
    vec3 maxColor = max(max(max(in0, in1), max(in2, in3)), in4);

    minColor = mix(minColor, min(min(min(in5, in6), min(in7, in8)), minColor), 0.5);
    maxColor = mix(maxColor, max(max(max(in5, in6), max(in7, in8)), maxColor), 0.5);

    vec3 preclamped = denoised;
    denoised = clamp(denoised, minColor, maxColor);

    mixRate = 1.0 / (1.0 / mixRate + 1.0);

    vec3 diff = denoised - preclamped;
    float clampAmount = dot(diff, diff);

    mixRate += clampAmount * 4.0;
    mixRate = clamp(mixRate, 0.05, 0.5);

    outColor = texture2D(u_color, v_uv) + vec4(decodePalYuv(denoised), mixRate);
}
