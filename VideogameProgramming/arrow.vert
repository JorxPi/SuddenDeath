#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 proj;

// tiempo global
uniform float t;
// máxima rotación en radianes (p.ej. ±0.2 rad = ±11°)
uniform float amplitude;
// ciclos por segundo
uniform float freq;

void main() {
    float saw = fract(t * freq);
    float angle = -saw * amplitude;
    mat2 R = mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );
    // aplicamos al vértice
    vec2 p = R * aPos;
    gl_Position = proj * model * vec4(p, 0.0, 1.0);
    TexCoords   = aTex;
}