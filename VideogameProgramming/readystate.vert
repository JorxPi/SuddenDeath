#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 proj;

// parámetros de pulso
uniform float t;
uniform float pulseFreq;  // ciclos por segundo
uniform float pulseAmp;   // amplitud [0..1]

void main() {
    // 1) Calcular pulso oscilante [-1..+1]
    float pulse = sin(2.0 * 3.141592653589793 * pulseFreq * t);
    // 2) Factor de escala [1-pulseAmp .. 1+pulseAmp]
    float s = 1.0 + pulseAmp * pulse;

    // 3) Aplicar escala + modelo + proyección
    vec2 scaledPos = aPos * s;
    gl_Position = proj * model * vec4(scaledPos, 0.0, 1.0);

    // 4) Pasar coordenadas de textura
    TexCoords = aTex;
}