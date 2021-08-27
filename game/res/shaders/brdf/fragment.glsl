#version 460 core

uniform vec3 u_origin;

uniform float u_seed;

uniform sampler2D u_textureNoise;

uniform sampler2D u_textureMat;
uniform sampler2D u_textureMag;

uniform sampler2D u_textureReservoir;

uniform sampler2D u_textureColor;
uniform sampler2D u_texturePosition;
uniform sampler2D u_textureNormal;
uniform sampler2D u_textureRoughness;

in vec2 v_uv;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outReservoir;

#define MAX_LIGHTS 10000
layout(std430, binding = 3) buffer Lights {
    uvec4 lightsCount;
    vec4  lightsColors[MAX_LIGHTS];
    vec4  lightsPositions[4*MAX_LIGHTS];
    uvec4 lightsIndices[4*MAX_LIGHTS];
};

float rand(float seed) {
    vec4 v = texture2D(u_textureNoise, v_uv + vec2(u_seed + seed));
    float r1 = v[int(4 * (u_seed + seed)) % 4];
    float r2 = v[int(4 * (u_seed + seed + r1)) % 4];
    float r3 = v[int(4 * (u_seed + seed + r1 + r2)) % 4];
    float r4 = v[int(4 * (u_seed + seed + r1 + r2 + r3)) % 4];
    return fract(r1 + r2 + r3 + r4);
}

vec3 toSRGB(vec3 v, float gamma) {
    return pow(v, vec3(1.0 / gamma));
}

struct LTC {
    vec3 position;
    float mag;
    mat3 invMat;
};

LTC ltcInit() {
    LTC ltc;

    vec3 normal = normalize(texture2D(u_textureNormal, v_uv).xyz);
    vec3 position = texture2D(u_texturePosition, v_uv).xyz;
    vec3 dir = normalize(position - u_origin);
    vec3 T1 = normalize(-dir - normalize(normal) * dot(-dir, normalize(normal)));
    vec3 T2 = cross(normalize(normal), T1);

    vec2 uv = vec2(texture2D(u_textureRoughness, v_uv).r, 0.6366197725 * acos(dot(vec3(0.0, 1.0, 0.0), -dir)));
    vec4 tex = texture2D(u_textureMat, uv);

    ltc.position = position;
    ltc.mag = texture2D(u_textureMag, uv).w;
    ltc.invMat = mat3(vec3(1.0, 0.0, tex.y), vec3(0.0, tex.z, 0.0), vec3(tex.w, 0.0, tex.x)) * transpose(mat3(T1, T2, normal));

    return ltc;
}

float ltcIntegrateEdge(vec3 p1, vec3 p2) {
    float theta = acos(dot(p1, p2));
    return cross(p1, p2).z * ((theta > 0.001) ? theta / sin(theta) : 1.0);
}

int ltcClipQuadToHorizon(inout vec3 L[5])
{
    // detect clipping config
    int config = 0;
    if (L[0].z > 0.0) config += 1;
    if (L[1].z > 0.0) config += 2;
    if (L[2].z > 0.0) config += 4;
    if (L[3].z > 0.0) config += 8;

    switch (config) {
    case 0: {  // clip all
        return 0;
    }
    case 1: { // V1 clip V2 V3 V4
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[3].z * L[0] + L[0].z * L[3];
        L[3] =  L[0];
        return 3;
    }
    case 2: { // V2 clip V1 V3 V4
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
        L[3] =  L[0];
        return 3;
    }
    case 3: { // V1 V2 clip V3 V4
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
        L[3] = -L[3].z * L[0] + L[0].z * L[3];
        L[4] =  L[0];
        return 4;
    }
    case 4: { // V3 clip V1 V2 V4
        L[0] = -L[3].z * L[2] + L[2].z * L[3];
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
        L[3] =  L[0];
        return 3;
    }
    case 5: { // V1 V3 clip V2 V4) impossible
        return 0;
    }
    case 6: { // V2 V3 clip V1 V4
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
        L[4] =  L[0];
        return 4;
    }
    case 7: { // V1 V2 V3 clip V4
        L[4] = -L[3].z * L[0] + L[0].z * L[3];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
        return 5;
    }
    case 8: { // V4 clip V1 V2 V3
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
        L[1] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] =  L[3];
        L[3] =  L[0];
        return 3;
    }
    case 9: { // V1 V4 clip V2 V3
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[2].z * L[3] + L[3].z * L[2];
        L[4] =  L[0];
        return 4;
    }
    case 10: { // V2 V4 clip V1 V3) impossible
        return 0;
    }
    case 11: { // V1 V2 V4 clip V3
        L[4] = L[3];
        L[3] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
        return 5;
    }
    case 12: { // V3 V4 clip V1 V2
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
        L[4] = L[0];
        return 4;
    }
    case 13: { // V1 V3 V4 clip V2
        L[4] = L[3];
        L[3] = L[2];
        L[2] = -L[1].z * L[2] + L[2].z * L[1];
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        return 5;
    }
    case 14: { // V2 V3 V4 clip V1
        L[4] = -L[0].z * L[3] + L[3].z * L[0];
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        return 5;
    }
    case 15: { // V1 V2 V3 V4
        L[4] = L[0];
        return 4;
    }
    }
}

float ltcIntegrateSample(uint sampleIdx, in LTC ltc) {
    vec3 L[5];
    L[0] = ltc.invMat * (lightsPositions[sampleIdx * 4 + 0].xyz - ltc.position);
    L[1] = ltc.invMat * (lightsPositions[sampleIdx * 4 + 1].xyz - ltc.position);
    L[2] = ltc.invMat * (lightsPositions[sampleIdx * 4 + 2].xyz - ltc.position);
    L[3] = ltc.invMat * (lightsPositions[sampleIdx * 4 + 3].xyz - ltc.position);

    int n = ltcClipQuadToHorizon(L);
    if (n == 0) return 0.0;

    L[0] = normalize(L[0]);
    L[1] = normalize(L[1]);
    L[2] = normalize(L[2]);
    L[3] = normalize(L[3]);
    L[4] = normalize(L[4]);

    float sum = 0.0;
    sum += ltcIntegrateEdge(L[0], L[1]);
    sum += ltcIntegrateEdge(L[1], L[2]);
    sum += ltcIntegrateEdge(L[2], L[3]);
    if (n >= 4) sum += ltcIntegrateEdge(L[3], L[4]);
    if (n == 5) sum += ltcIntegrateEdge(L[4], L[0]);

    return 0.15915494312177461029 * ltc.mag * max(sum, 0.0);
}

struct Reservoir {
    uint idx;
    uint m;
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
    return vec2(uintBitsToFloat((r.idx << 16) + r.m), r.sumw);
}

Reservoir reservoirDecode(vec2 v) {
    Reservoir r;
    uint idx_m = floatBitsToUint(v.x);
    r.idx = idx_m >> 16;
    r.m = idx_m & uint(0xFFFF);
    r.sumw = v.y;
    return r;
}

void reservoirUpdate(inout Reservoir r, uint idx, float w, float seed) {
    r.sumw += w;
    r.m++;
    if (r.m == 0 || rand(0.3660739151829132 + seed) <= w / r.sumw) {
        r.idx = idx;
    }
}

void reservoirMerge(inout Reservoir lhs, in Reservoir rhs, in LTC ltc, float seed) {
    reservoirUpdate(lhs, rhs.idx, rhs.sumw, seed);
    lhs.m += rhs.m - 1;
}

void RIS(inout Reservoir r, in LTC ltc) {
    const int M = 1;

    for (int i = 0; i < M; i++) {
        float seed = 0.62717936095487 + float(i) / float(M);
        //uint candidate = 2 + 4*10*6 + 4*5;
        uint candidate = int(rand(seed) * lightsCount.x) % lightsCount.x;
        reservoirUpdate(r, candidate, ltcIntegrateSample(candidate, ltc), seed);
    }
}

void main() {
    LTC ltc = ltcInit();

    Reservoir reservoir = reservoirInit();
    RIS(reservoir, ltc);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0,  0), 0).xy), ltc, 0.4228584333060338);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2(-2,  0), 0).xy), ltc, 0.1314829920117552);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 2,  0), 0).xy), ltc, 0.7972877170150268);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0, -2), 0).xy), ltc, 0.6601296319005907);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0,  2), 0).xy), ltc, 0.4979573192540756);
    reservoir.sumw = ltcIntegrateSample(reservoir.idx, ltc);
    reservoir.m = 1;

    vec3 color = reservoir.sumw * lightsColors[reservoir.idx].rgb;

    /*bool asd = (int(rand(0.0) * lightsCount.x) % lightsCount.x) == 262;
    outColor = vec4(vec3(asd ? 1.0 : 0.0), 1.0);*/
    outColor = vec4(toSRGB(texture2D(u_textureColor, v_uv).rgb * (vec3(0.05) + 0.95 * color), 2.0), 1.0);
    outReservoir = reservoirEncode(reservoir);
}
