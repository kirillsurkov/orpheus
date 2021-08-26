#version 330 core

uniform vec3 u_color;

out vec4 outColor;

void main() {
    outColor = vec4(pow(u_color, vec3(1.0 / 2.0)), 1);
}
