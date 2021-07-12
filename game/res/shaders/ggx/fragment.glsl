#version 330 core

uniform vec3 u_origin;
uniform float u_roughness;
uniform sampler2D u_textureMat;
uniform sampler2D u_textureMag;
uniform sampler2D u_textureReservoir[2];
uniform int u_lightPointsCount;
uniform vec3 u_lightPoints[400];
uniform int u_lightIndicesCount;
uniform ivec2 u_lightIndices[400];
uniform vec3 u_lightColors[200];
uniform float u_seed;

in vec3 v_normal;
in vec3 v_position;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outReservoir12;
layout (location = 2) out vec4 outReservoir34;

float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

vec3 ToSRGB(vec3 v) {
    return pow(v, vec3(1.0 / 2.0));
}

float integrateEdge(vec3 v1, vec3 v2) {
    float theta = acos(dot(v1, v2));
    return cross(v1, v2).z * ((theta > 0.001) ? theta / sin(theta) : 1.0);
}

float integrateSample(int idx, float mag, in mat3 invMat) {
    float sum = 0.0;
    for (int i = 0; i < 4; i++) {
        ivec2 index = u_lightIndices[idx * 4 + i];
        vec3 p1 = normalize(invMat * (u_lightPoints[index.x] - v_position));
        vec3 p2 = normalize(invMat * (u_lightPoints[index.y] - v_position));
        sum += integrateEdge(p1, p2);
    }
    return 0.15915494312177461029 * mag * abs(sum);
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

void reservoirUpdate(inout Reservoir r, int idx, float w) {
    r.sumw += w;
    r.m++;
    if (rand(gl_FragCoord.xy + 0.123456) < (w / r.sumw)) {
        r.idx = idx;
    }
}

void reservoirMerge(inout Reservoir lhs, in Reservoir rhs) {
    int m = lhs.m + rhs.m;
    reservoirUpdate(lhs, rhs.idx, rhs.sumw);
    lhs.m = m;
}

Reservoir reservoirDecode(vec2 v) {
    Reservoir r;
    int idx_m = floatBitsToInt(v.x);
    r.idx = idx_m >> 16;
    r.m = idx_m & 0xFFFF;
    r.sumw = v.y;
    return r;
}

vec2 reservoirEncode(in Reservoir r) {
    return vec2(intBitsToFloat((r.idx << 16) + r.m), r.sumw);
}

void reservoirMergeNeighbours(inout Reservoir dst, sampler2D tex) {
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2( 0,  0), 0).xy));
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2( 0,  0), 0).zw));
    /*
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2(-1,  0), 0).xy));
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2(-1,  0), 0).zw));
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2( 1,  0), 0).xy));
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2( 1,  0), 0).zw));
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2( 0, -1), 0).xy));
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2( 0, -1), 0).zw));
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2( 0,  1), 0).xy));
    reservoirMerge(dst, reservoirDecode(texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2( 0,  1), 0).zw));
    */
}

Reservoir RIS(float mag, in mat3 invMat, float seed) {
    const int M = 16;

    Reservoir r = reservoirInit();

    for (int i = 0; i < M; i++) {
        int candidate = int(rand(gl_FragCoord.xy + seed * float(i + 1) / float(M)) * (float(u_lightPointsCount) / 4.0));
        reservoirUpdate(r, candidate, integrateSample(candidate, mag, invMat));
    }

    return r;
}

void main() {
    vec3 dir = normalize(v_position - u_origin);
    vec3 T1 = normalize(-dir - normalize(v_normal) * dot(-dir, normalize(v_normal)));
    vec3 T2 = cross(normalize(v_normal), T1);

    vec2 uv = vec2(u_roughness, 0.6366197725 * acos(dot(vec3(0.0, 1.0, 0.0), -dir)));
    float mag = texture2D(u_textureMag, uv).w;
    vec4 tex = texture2D(u_textureMat, uv);
    mat3 invMat = mat3(vec3(1.0, 0.0, tex.y), vec3(0.0, tex.z, 0.0), vec3(tex.w, 0.0, tex.x)) * transpose(mat3(T1, T2, normalize(v_normal)));

    Reservoir reservoir = reservoirInit();
    reservoirMergeNeighbours(reservoir, u_textureReservoir[0]);
    reservoirMergeNeighbours(reservoir, u_textureReservoir[1]);

    float spec = integrateSample(reservoir.idx, mag, invMat);
    float diff = 0.0;

    vec3 colorSpec = vec3(u_lightColors[reservoir.idx]);
    vec3 colorDiff = vec3(0.0);

    outColor = vec4(ToSRGB(vec3(colorSpec * spec + colorDiff * diff)), 1.0);
    outReservoir12 = vec4(reservoirEncode(RIS(mag, invMat, 0.0)), reservoirEncode(RIS(mag, invMat, 0.25)));
    outReservoir34 = vec4(reservoirEncode(RIS(mag, invMat, 0.5)), reservoirEncode(RIS(mag, invMat, 0.75)));
}
