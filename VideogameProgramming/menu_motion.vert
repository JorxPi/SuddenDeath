#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aTex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 proj;

// built-in time (you already push `t` in your DrawSprite)
uniform float t;

// per-instance parameters
uniform float ampScale;   // e.g. 0.05  ±5% size
uniform float ampAngle;   // radians, e.g. 0.05  ±2.8
uniform float freq;       // oscillations per second
uniform float phase;      // phase offset in radians

void main(){
    // compute a little oscillating scale and rotation
    float s = 1.0 + ampScale * sin(freq * t + phase);
    float a =     ampAngle * sin(freq * t + phase);

    // build a 2×2 rotation matrix
    mat2 rot = mat2(
        cos(a), -sin(a),
        sin(a),  cos(a)
    );

    // apply scale+rot around the sprite's local origin
    vec2 pos = rot * (aPos * s);

    gl_Position  = proj * model * vec4(pos, 0.0, 1.0);
    TexCoords    = aTex;
}
