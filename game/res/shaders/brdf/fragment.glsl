#version 430 core

const float PI = 3.141592653;

const uint MAX_LIGHTS = 10000;

const float LUT_SIZE  = 64.0;
const float LUT_SCALE = (LUT_SIZE - 1.0) / LUT_SIZE;
const float LUT_BIAS  = 0.5 / LUT_SIZE;

uniform vec3 u_origin;
uniform vec3 u_forward;
uniform vec3 u_right;

uniform float u_seed;
uniform bool u_useMotion;

uniform sampler2D u_textureNoise;

uniform sampler2D u_textureLtc1;
uniform sampler2D u_textureLtc2;

uniform sampler2D u_textureReservoir;

uniform sampler2D u_textureColor;
uniform sampler2D u_texturePosition;
uniform sampler2D u_textureNormal;
uniform sampler2D u_textureRoughness;
uniform sampler2D u_textureMotion;

in vec2 v_uv;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outReservoir;

layout(std430, binding = 3) buffer Lights {
    uvec4 lightsCount;
    uvec4 lightsTypes[MAX_LIGHTS];
    vec4  lightsColors[MAX_LIGHTS];
    vec4  lightsPositions[4*MAX_LIGHTS];
};

float rand(float seed) {
    vec4 v = texture2D(u_textureNoise, v_uv + vec2(u_seed + seed));
    float r1 = v[int(4 * (u_seed + seed)) % 4];
    float r2 = v[int(4 * (u_seed + seed + r1)) % 4];
    float r3 = v[int(4 * (u_seed + seed + r1 + r2)) % 4];
    float r4 = v[int(4 * (u_seed + seed + r1 + r2 + r3)) % 4];
    return fract(r1 + r2 + r3 + r4);
}

struct LTC {
    float coeff;
    vec3 position;
    mat3 mat;
    mat3 matR;
    mat3 matInv;
};

LTC ltcInit(float roughness) {
    LTC ltc;

    vec3 normal = normalize(texture2D(u_textureNormal, v_uv).xyz);
    vec3 position = texture2D(u_texturePosition, v_uv).xyz;
    vec3 dir = normalize(position - u_origin);

    float ndotv = clamp(dot(normal, -dir), 0.0, 1.0);
    vec3 T1 = normalize(-dir - normal * dot(-dir, normal));
    vec3 T2 = cross(normal, T1);
    vec2 uv = vec2(roughness, sqrt(1.0 - ndotv)) * LUT_SCALE + LUT_BIAS;

    vec4 tex1 = texture2D(u_textureLtc1, uv);
    vec4 tex2 = texture2D(u_textureLtc2, uv);

    ltc.coeff = tex2.r;
    ltc.position = position;
    ltc.mat = mat3(vec3(tex1.x, 0.0, tex1.y), vec3(0.0, 1.0, 0.0), vec3(tex1.z, 0.0, tex1.w));
    ltc.matR = transpose(mat3(T1, T2, normal));
    ltc.matInv = ltc.mat * ltc.matR;

    return ltc;
}

vec3 ltcIntegrateEdgeVec(vec3 v1, vec3 v2) {
    float x = dot(v1, v2);
    float y = abs(x);

    float a = 0.8543985 + (0.4965155 + 0.0145206 * y) * y;
    float b = 3.4175940 + (4.1616724 + y) * y;
    float v = a / b;

    float theta_sintheta = (x > 0.0) ? v : 0.5 * inversesqrt(max(1.0 - x * x, 1e-7)) - v;

    return cross(v1, v2) * theta_sintheta;
}

float ltcIntegrateSampleQuad(uint sampleIdx, in LTC ltc) {
    vec3 p1 = lightsPositions[sampleIdx * 4 + 0].xyz - ltc.position;
    vec3 p2 = lightsPositions[sampleIdx * 4 + 1].xyz - ltc.position;
    vec3 p3 = lightsPositions[sampleIdx * 4 + 2].xyz - ltc.position;
    vec3 p4 = lightsPositions[sampleIdx * 4 + 3].xyz - ltc.position;

    if (dot(p1, cross(p2 - p1, p4 - p1)) < 0.0) {
        return 0.0;
    }

    p1 = normalize(ltc.matInv * p1);
    p2 = normalize(ltc.matInv * p2);
    p3 = normalize(ltc.matInv * p3);
    p4 = normalize(ltc.matInv * p4);

    vec3 vsum = vec3(0.0);
    vsum += ltcIntegrateEdgeVec(p1, p2);
    vsum += ltcIntegrateEdgeVec(p2, p3);
    vsum += ltcIntegrateEdgeVec(p3, p4);
    vsum += ltcIntegrateEdgeVec(p4, p1);

    float len = length(vsum);
    float z = vsum.z / len;

    return ltc.coeff * len * texture2D(u_textureLtc2, vec2(z * 0.5 + 0.5, len) * LUT_SCALE + vec2(LUT_BIAS)).w;
}

vec3 SolveCubic(vec4 Coefficient) {
    Coefficient.xyz /= Coefficient.w;
    Coefficient.yz /= 3.0;

    float A = Coefficient.w;
    float B = Coefficient.z;
    float C = Coefficient.y;
    float D = Coefficient.x;

    vec3 Delta = vec3(
        -Coefficient.z*Coefficient.z + Coefficient.y,
        -Coefficient.y*Coefficient.z + Coefficient.x,
        dot(vec2(Coefficient.z, -Coefficient.y), Coefficient.xy)
    );

    float Discriminant = dot(vec2(4.0*Delta.x, -Delta.y), Delta.zy);

    vec3 RootsA, RootsD;

    vec2 xlc, xsc;

    {
        float A_a = 1.0;
        float C_a = Delta.x;
        float D_a = -2.0*B*Delta.x + Delta.y;

        float Theta = atan(sqrt(Discriminant), -D_a)/3.0;

        float x_1a = 2.0*sqrt(-C_a)*cos(Theta);
        float x_3a = 2.0*sqrt(-C_a)*cos(Theta + (2.0/3.0)*PI);

        float xl;
        if ((x_1a + x_3a) > 2.0*B)
            xl = x_1a;
        else
            xl = x_3a;

        xlc = vec2(xl - B, A);
    }

    {
        float A_d = D;
        float C_d = Delta.z;
        float D_d = -D*Delta.y + 2.0*C*Delta.z;

        float Theta = atan(D*sqrt(Discriminant), -D_d)/3.0;

        float x_1d = 2.0*sqrt(-C_d)*cos(Theta);
        float x_3d = 2.0*sqrt(-C_d)*cos(Theta + (2.0/3.0)*PI);

        float xs;
        if (x_1d + x_3d < 2.0*C)
            xs = x_1d;
        else
            xs = x_3d;

        xsc = vec2(-D, xs + C);
    }

    float E =  xlc.y*xsc.y;
    float F = -xlc.x*xsc.y - xlc.y*xsc.x;
    float G =  xlc.x*xsc.x;

    vec2 xmc = vec2(C*F - B*G, -B*F + C*E);

    vec3 Root = vec3(xsc.x/xsc.y, xmc.x/xmc.y, xlc.x/xlc.y);

    if (Root.x < Root.y && Root.x < Root.z)
        Root.xyz = Root.yxz;
    else if (Root.z < Root.x && Root.z < Root.y)
        Root.xyz = Root.xzy;

    return Root;
}

float ltcIntegrateSampleSphere(uint sampleIdx, in LTC ltc) {
    vec3 sphc = lightsPositions[sampleIdx * 4 + 0].xyz;
    vec3 ex = -u_right;
    vec3 ey = cross(u_forward, u_right);
    ey.y = -ey.y;

    float radius = lightsPositions[sampleIdx * 4 + 1].x;
    ex *= radius;
    ey *= radius;

    vec3 p1 = ltc.matR * ((sphc - ex - ey) - ltc.position);
    vec3 p2 = ltc.matR * ((sphc + ex - ey) - ltc.position);
    vec3 p3 = ltc.matR * ((sphc + ex + ey) - ltc.position);

    vec3 C  = ltc.mat * (0.5 * (p1 + p3));
    vec3 V1 = ltc.mat * (0.5 * (p2 - p3));
    vec3 V2 = ltc.mat * (0.5 * (p2 - p1));

    if (dot(cross(V1, V2), C) < 0.0) {
        return 0.0;
    }

    float a, b;
    float d11 = dot(V1, V1);
    float d22 = dot(V2, V2);
    float d12 = dot(V1, V2);
    if (abs(d12)/sqrt(d11*d22) > 0.0001)
    {
        float tr = d11 + d22;
        float det = -d12*d12 + d11*d22;

        det = sqrt(det);
        float u = 0.5*sqrt(tr - 2.0*det);
        float v = 0.5*sqrt(tr + 2.0*det);
        float e_max = (u + v) * (u + v);
        float e_min = (u - v) * (u - v);

        vec3 V1_, V2_;

        if (d11 > d22)
        {
            V1_ = d12*V1 + (e_max - d11)*V2;
            V2_ = d12*V1 + (e_min - d11)*V2;
        }
        else
        {
            V1_ = d12*V2 + (e_max - d22)*V1;
            V2_ = d12*V2 + (e_min - d22)*V1;
        }

        a = 1.0 / e_max;
        b = 1.0 / e_min;
        V1 = normalize(V1_);
        V2 = normalize(V2_);
    } else {
        a = 1.0 / dot(V1, V1);
        b = 1.0 / dot(V2, V2);
        V1 *= sqrt(a);
        V2 *= sqrt(b);
    }

    vec3 V3 = cross(V1, V2);
    if (dot(C, V3) < 0.0)
        V3 *= -1.0;

    float L  = dot(V3, C);
    float x0 = dot(V1, C) / L;
    float y0 = dot(V2, C) / L;

    float E1 = inversesqrt(a);
    float E2 = inversesqrt(b);

    a *= L*L;
    b *= L*L;

    float c0 = a*b;
    float c1 = a*b*(1.0 + x0*x0 + y0*y0) - a - b;
    float c2 = 1.0 - a*(1.0 + x0*x0) - b*(1.0 + y0*y0);
    float c3 = 1.0;

    vec3 roots = SolveCubic(vec4(c0, c1, c2, c3));
    float e1 = roots.x;
    float e2 = roots.y;
    float e3 = roots.z;

    vec3 avgDir = vec3(a*x0/(a - e2), b*y0/(b - e2), 1.0);

    mat3 rotate = mat3(V1, V2, V3);

    avgDir = rotate*avgDir;
    avgDir = normalize(avgDir);

    float L1 = sqrt(-e2/e3);
    float L2 = sqrt(-e2/e1);

    float formFactor = L1*L2*inversesqrt((1.0 + L1*L1)*(1.0 + L2*L2));

    return formFactor * texture(u_textureLtc2, vec2(avgDir.z*0.5 + 0.5, formFactor) * LUT_SCALE + LUT_BIAS).w;
}

float ltcIntegrateSample(uint sampleIdx, in LTC ltc) {
    switch (lightsTypes[sampleIdx].x) {
    case 0: return ltcIntegrateSampleQuad(sampleIdx, ltc);
    case 1: return ltcIntegrateSampleSphere(sampleIdx, ltc);
    }
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

void reservoirMerge(inout Reservoir lhs, in Reservoir rhs, float seed) {
    reservoirUpdate(lhs, rhs.idx, rhs.sumw, seed);
    lhs.m += rhs.m - 1;
}

void RIS(inout Reservoir r, in LTC ltcSpec) {
    const int M = 1;

    for (int i = 0; i < M; i++) {
        float seed = 0.62717936095487 + float(i) / float(M);
        uint candidate = int(rand(seed) * lightsCount.x) % lightsCount.x;
        reservoirUpdate(r, candidate, ltcIntegrateSample(candidate, ltcSpec), seed);
    }
}

void main() {
    float roughness = texture2D(u_textureRoughness, v_uv).r;

    LTC ltcSpec = ltcInit(roughness);
    LTC ltcDiff = ltcInit(1.0 - roughness);

    vec2 m = (u_useMotion ? -0.5 : 0.0) * texelFetch(u_textureMotion, ivec2(gl_FragCoord.xy), 0).xy;

    Reservoir reservoir = reservoirInit();
    RIS(reservoir, ltcSpec);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2(m.x,     m.y), 0).xy), 0.4228584333060338);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2(m.x - 2, m.y), 0).xy), 0.1314829920117552);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2(m.x + 2, m.y), 0).xy), 0.7972877170150268);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2(m.x,     m.y - 2), 0).xy), 0.6601296319005907);
    reservoirMerge(reservoir, reservoirDecode(texelFetch(u_textureReservoir, ivec2(gl_FragCoord.xy) + ivec2(m.x,     m.y + 2), 0).xy), 0.4979573192540756);
    reservoir.sumw = ltcIntegrateSample(reservoir.idx, ltcSpec);
    reservoir.m = 1;

    vec3 color = reservoir.sumw * lightsColors[reservoir.idx].rgb;

    outColor = vec4(texture2D(u_textureColor, v_uv).rgb * (vec3(0.05) + 0.95 * color), 1.0);
    outReservoir = reservoirEncode(reservoir);
}
