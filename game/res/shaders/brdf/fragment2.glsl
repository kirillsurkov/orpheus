#version 330 core

uniform vec3 u_origin;
uniform float u_roughness;
uniform sampler2D u_textureMat;
uniform sampler2D u_textureMag;
uniform sampler2D u_textureTemporal;
uniform sampler2D u_textureReservoir;
uniform vec3 u_lightPoints[500];
uniform int u_lightPointsCount;
uniform ivec2 u_lightIndices[500];
uniform int u_lightIndicesCount;
uniform float u_seed;

in vec3 v_normal;
in vec3 v_position;

layout (location=0) out vec4 outColor;

const vec3 colorSpec = vec3(1.0, 1.0, 1.0);
const vec3 colorDiff = vec3(1.0, 1.0, 1.0);

vec3 ToSRGB(vec3 v) {
    return pow(v, vec3(1.0 / 2.0));
}

float integrateEdge(vec3 v1, vec3 v2) {
    float theta = acos(dot(v1, v2));
    return cross(v1, v2).z * ((theta > 0.001) ? theta/sin(theta) : 1.0);
}

float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float ltcEval(vec3 T1, vec3 T2, in mat3 m_inv) {
    m_inv *= transpose(mat3(T1, T2, v_normal));
    int offset = 4 * (int(500 * rand(u_seed + gl_FragCoord.xy)) / 4);
    int iterations = min(u_lightIndicesCount, 32);
    float sum = 0.0;
    for (int i = 0; i < iterations; i++) {
        ivec2 index = u_lightIndices[(i + offset) % u_lightIndicesCount];
        vec3 p1 = normalize(m_inv * (u_lightPoints[index.x] - v_position));
        vec3 p2 = normalize(m_inv * (u_lightPoints[index.y] - v_position));
        sum += integrateEdge(p1, p2);
    }
    return 0.15915494312177461029 * abs(sum);
}

void main() {
    vec3 dir = normalize(v_position - u_origin);
    vec3 T1 = normalize(-dir - v_normal * dot(-dir, v_normal));
    vec3 T2 = cross(v_normal, T1);
    vec2 uv = vec2(u_roughness, 0.6366197725 * acos(dot(vec3(0.0, 1.0, 0.0), -dir)));
    vec4 t = texture2D(u_textureMat, uv);
    float diff = ltcEval(T1, T2, mat3(1.0));
    float spec = ltcEval(T1, T2, mat3(vec3(1.0, 0.0, t.y), vec3(0.0, t.z, 0.0), vec3(t.w, 0.0, t.x)));
    outColor = vec4(ToSRGB(vec3(texture2D(u_textureMag, uv).w * colorSpec * spec * (1.0 - u_roughness) + colorDiff * diff * u_roughness)), 1.0);
}
