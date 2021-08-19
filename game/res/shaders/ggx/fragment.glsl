#version 330 core

uniform vec3 u_origin;
uniform float u_roughness;
uniform sampler2D u_textureMat;
uniform sampler2D u_textureMag;
uniform sampler2D u_textureReservoir;
uniform int u_lightPointsCount;
uniform vec3 u_lightPoints[400];
uniform int u_lightIndicesCount;
uniform ivec2 u_lightIndices[400];
uniform vec3 u_lightColors[200];
uniform float u_seed;

in vec3 v_normal;
in vec3 v_position;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outReservoir;

float rand(float n) {
    return fract(sin(dot(gl_FragCoord.xy + n, vec2(12.9898, 4.1414))) * 43758.5453);
}

vec3 toSRGB(vec3 v, float gamma) {
    return pow(v, vec3(1.0 / gamma));
}

struct LTC {
    float mag;
    mat3 invMat;
};

LTC ltcInit() {
    LTC ltc;

    vec3 dir = normalize(v_position - u_origin);
    vec3 T1 = normalize(-dir - normalize(v_normal) * dot(-dir, normalize(v_normal)));
    vec3 T2 = cross(normalize(v_normal), T1);

    vec2 uv = vec2(u_roughness, 0.6366197725 * acos(dot(vec3(0.0, 1.0, 0.0), -dir)));
    vec4 tex = texture2D(u_textureMat, uv);

    ltc.mag = texture2D(u_textureMag, uv).w;
    ltc.invMat = mat3(vec3(1.0, 0.0, tex.y), vec3(0.0, tex.z, 0.0), vec3(tex.w, 0.0, tex.x)) * transpose(mat3(T1, T2, normalize(v_normal)));

    return ltc;
}

float ltcIntegrateSample(int sampleIdx, in LTC ltc) {
    float sum = 0.0;
    for (int i = 0; i < 4; i++) {
        ivec2 index = u_lightIndices[sampleIdx * 4 + i];
        vec3 p1 = normalize(ltc.invMat * (u_lightPoints[index.x] - v_position));
        vec3 p2 = normalize(ltc.invMat * (u_lightPoints[index.y] - v_position));
        float theta = acos(dot(p1, p2));
        sum += cross(p1, p2).z * ((theta > 0.001) ? theta / sin(theta) : 1.0);
    }
    return 0.15915494312177461029 * ltc.mag * abs(sum);
}

struct Reservoir {
    int idx;
    int m;
    float sumw;
};

Reservoir reservoirInit() {
    Reservoir r;
    r.idx = 0;
    r.m = 0;
    r.sumw = 0.0;
    return r;
}

vec2 reservoirEncode(in Reservoir r) {
    return vec2(intBitsToFloat((r.idx << 16) + r.m), r.sumw);
}

Reservoir reservoirDecode(vec2 v) {
    Reservoir r;
    int idx_m = floatBitsToInt(v.x);
    r.idx = idx_m >> 16;
    r.m = idx_m & 0xFFFF;
    r.sumw = v.y;
    return r;
}

void reservoirUpdate(inout Reservoir r, int idx, float w) {
    r.sumw += w;
    r.m++;
    if (r.m == 0 || rand(0.0) <= w / r.sumw) {
        r.idx = idx;
    }
}

void reservoirMerge(inout Reservoir lhs, in Reservoir rhs) {
    reservoirUpdate(lhs, rhs.idx, rhs.sumw);
    lhs.m += rhs.m - 1;
}

Reservoir RIS(inout Reservoir r, in LTC ltc, float seed) {
    const int M = 2;

    for (int i = 0; i < M; i++) {
        int candidate = int(rand(seed + i / M) * (float(u_lightPointsCount) / 4.0));
        reservoirUpdate(r, candidate, ltcIntegrateSample(candidate, ltc));
    }

    return r;
}

void main() {
    LTC ltc = ltcInit();

    Reservoir reservoir = RIS(reservoirInit(), ltc, u_seed);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0,  0), 0).xy));
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2(-1,  0), 0).xy));
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 1,  0), 0).xy));
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0, -1), 0).xy));
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0,  1), 0).xy));
    reservoir.sumw = ltcIntegrateSample(reservoir.idx, ltc);
    reservoir.m = 1;

    float spec = reservoir.sumw;
    float diff = 0.0;

    vec3 colorSpec = u_lightColors[reservoir.idx];
    vec3 colorDiff = vec3(0.0);

    outColor = vec4(toSRGB(vec3(colorSpec * spec + colorDiff * diff), 2.0), 1.0);
    outReservoir = reservoirEncode(reservoir);
}
