#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D sprite;
uniform float t;          // tiempo total
uniform float pulseFreq;  // ciclos por segundo
uniform float pulseAmp;   // amplitud del pulso (0–1)
uniform vec3  glowColor;  // color del glow

void main(){
    // 1) sample base
    vec4 base = texture(sprite, TexCoords);

    // 2) pulso oscilante [0..1]
    float pulse = 0.5 + 0.5 * sin( t * 2.0 * 3.14159 * pulseFreq );
    // aplicamos amplitud
    float glow = mix(1.0, 1.0 + pulseAmp, pulse);

    // 3) rim highlight: más fuerte en los bordes
    vec2 center = vec2(0.5);
    float dist = distance(TexCoords, center);
    // suave transición en los bordes del sprite
    float rim = smoothstep(0.45, 0.5, dist);

    // 4) mezclamos glow y rim
    vec3 color = base.rgb * glow 
               + glowColor * rim * pulse * pulseAmp;

    FragColor = vec4(color, base.a * glow);
}
