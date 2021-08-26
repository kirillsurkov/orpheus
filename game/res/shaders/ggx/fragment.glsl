#version 460 core

uniform vec3 u_origin;
uniform float u_roughness;
uniform sampler2D u_textureMat;
uniform sampler2D u_textureMag;
uniform sampler2D u_textureNoise;
uniform sampler2D u_textureReservoir;
uniform float u_seed;

uniform sampler2D u_textureFloorColor;
uniform sampler2D u_textureFloorNormal;
uniform sampler2D u_textureFloorRoughness;

in vec3 v_normal;
in vec3 v_position;
in vec2 v_uv;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outReservoir;

#define MAX_LIGHTS 10000
layout(std430, binding = 3) buffer Lights {
    ivec4 lightsCount;
    vec4  lightsColors[MAX_LIGHTS];
    vec4  lightsPositions[4*MAX_LIGHTS];
    ivec4 lightsIndices[4*MAX_LIGHTS];
};

float rand(float n) {
    vec3 c = texture2D(u_textureNoise, vec2(gl_FragCoord.xy / vec2(1024, 1024) + n + u_seed)).rgb;
    return c.x * 0.6030262432549202 + c.y * 0.27573879843779947 + c.z * 0.12123495830728026;
}

vec3 toSRGB(vec3 v, float gamma) {
    return pow(v, vec3(1.0 / gamma));
}

vec3 floorGetColor() {
    vec3 c = texture2D(u_textureFloorColor, v_uv).rgb;
    return vec3((c.x + c.y + c.z) / 3.0);
}

vec3 floorGetNormal() {
    return texture2D(u_textureFloorNormal, v_uv).xzy * 2 - 1;
}

float floorGetRoughness() {
    return texture2D(u_textureFloorRoughness, v_uv).r;
}

struct LTC {
    float mag;
    mat3 invMat;
};

LTC ltcInit() {
    LTC ltc;

    //vec3 normal = normalize(v_normal);
    vec3 normal = normalize(floorGetNormal());
    vec3 dir = normalize(v_position - u_origin);
    vec3 T1 = normalize(-dir - normalize(normal) * dot(-dir, normalize(normal)));
    vec3 T2 = cross(normalize(normal), T1);

    vec2 uv = vec2(floorGetRoughness(), 0.6366197725 * acos(dot(vec3(0.0, 1.0, 0.0), -dir)));
    vec4 tex = texture2D(u_textureMat, uv);

    ltc.mag = texture2D(u_textureMag, uv).w;
    ltc.invMat = mat3(vec3(1.0, 0.0, tex.y), vec3(0.0, tex.z, 0.0), vec3(tex.w, 0.0, tex.x)) * transpose(mat3(T1, T2, normal));

    return ltc;
}

void ClipQuadToHorizon(inout vec3 L[5], out int n)
{
    // detect clipping config
    int config = 0;
    if (L[0].z > 0.0) config += 1;
    if (L[1].z > 0.0) config += 2;
    if (L[2].z > 0.0) config += 4;
    if (L[3].z > 0.0) config += 8;

    // clip
    n = 0;

    if (config == 0)
    {
        // clip all
    }
    else if (config == 1) // V1 clip V2 V3 V4
    {
        n = 3;
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[3].z * L[0] + L[0].z * L[3];
    }
    else if (config == 2) // V2 clip V1 V3 V4
    {
        n = 3;
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
    }
    else if (config == 3) // V1 V2 clip V3 V4
    {
        n = 4;
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
        L[3] = -L[3].z * L[0] + L[0].z * L[3];
    }
    else if (config == 4) // V3 clip V1 V2 V4
    {
        n = 3;
        L[0] = -L[3].z * L[2] + L[2].z * L[3];
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
    }
    else if (config == 5) // V1 V3 clip V2 V4) impossible
    {
        n = 0;
    }
    else if (config == 6) // V2 V3 clip V1 V4
    {
        n = 4;
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
    }
    else if (config == 7) // V1 V2 V3 clip V4
    {
        n = 5;
        L[4] = -L[3].z * L[0] + L[0].z * L[3];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
    }
    else if (config == 8) // V4 clip V1 V2 V3
    {
        n = 3;
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
        L[1] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] =  L[3];
    }
    else if (config == 9) // V1 V4 clip V2 V3
    {
        n = 4;
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[2].z * L[3] + L[3].z * L[2];
    }
    else if (config == 10) // V2 V4 clip V1 V3) impossible
    {
        n = 0;
    }
    else if (config == 11) // V1 V2 V4 clip V3
    {
        n = 5;
        L[4] = L[3];
        L[3] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
    }
    else if (config == 12) // V3 V4 clip V1 V2
    {
        n = 4;
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
    }
    else if (config == 13) // V1 V3 V4 clip V2
    {
        n = 5;
        L[4] = L[3];
        L[3] = L[2];
        L[2] = -L[1].z * L[2] + L[2].z * L[1];
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
    }
    else if (config == 14) // V2 V3 V4 clip V1
    {
        n = 5;
        L[4] = -L[0].z * L[3] + L[3].z * L[0];
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
    }
    else if (config == 15) // V1 V2 V3 V4
    {
        n = 4;
    }

    if (n == 3)
        L[3] = L[0];
    if (n == 4)
        L[4] = L[0];
}

float ltcIntegrateSample(int sampleIdx, in LTC ltc) {
    ivec2 index = ivec2(lightsIndices[sampleIdx * 4].xy);

    vec3 L[5];
    L[0] = ltc.invMat * (lightsPositions[index.x + 0].xyz - v_position);
    L[1] = ltc.invMat * (lightsPositions[index.x + 1].xyz - v_position);
    L[2] = ltc.invMat * (lightsPositions[index.x + 2].xyz - v_position);
    L[3] = ltc.invMat * (lightsPositions[index.x + 3].xyz - v_position);

    int n;
    ClipQuadToHorizon(L, n);
    if (n == 0) return 0.0;

    L[0] = normalize(L[0]);
    L[1] = normalize(L[1]);
    L[2] = normalize(L[2]);
    L[3] = normalize(L[3]);
    L[4] = normalize(L[4]);

    float sum = 0.0;
    for (int i = 0; i < 5; i++) {
        if (i == 3 && n < 4) continue;
        if (i == 4 && n < 5) continue;

        vec3 p1 = L[i];
        vec3 p2 = L[(i + 1) % 5];
        float theta = acos(dot(p1, p2));
        sum += cross(p1, p2).z * ((theta > 0.001) ? theta / sin(theta) : 1.0);
    }
    return 0.15915494312177461029 * ltc.mag * max(sum, 0.0);
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

void RIS(inout Reservoir r, in LTC ltc) {
    const int M = 2;

    for (int i = 0; i < M; i++) {
        int x = int(rand(u_seed + i / M) * lightsCount.x);
        int candidate = x % lightsCount.x;
        reservoirUpdate(r, candidate, ltcIntegrateSample(candidate, ltc));
    }
}

void main() {
    LTC ltc = ltcInit();

    Reservoir reservoir = reservoirInit();
    RIS(reservoir, ltc);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0,  0), 0).xy));
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2(-3,  0), 0).xy));
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 3,  0), 0).xy));
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0, -3), 0).xy));
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2( 0,  3), 0).xy));
    reservoir.sumw = ltcIntegrateSample(reservoir.idx, ltc);
    reservoir.m = 1;

    vec3 color = reservoir.sumw * lightsColors[reservoir.idx].rgb;

    outColor = vec4(toSRGB(floorGetColor() * (vec3(0.0) + 1.0 * color), 2.0), 1.0);
    outReservoir = reservoirEncode(reservoir);
}
