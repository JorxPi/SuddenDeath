#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 ruletaColor1;
uniform vec3 ruletaColor2;
uniform float atac1;
uniform float atac2;

void main() {
    //vec3 ruletaColor1 = vec3(1.0, 0.0, 0.0); // Red
    //vec3 ruletaColor2 = vec3(0.0, 1.0, 0.0); // Green
    
    //float atac1 = 1.0;
    //float atac2 = 3.0;

    // Assuming TexCoords is normalized (0 to 1)
    vec2 uv = TexCoords;
    const float PI = 3.1415926535897932384626433832795;
    vec3 color;
    float alpha = 1.0;
    vec2 center = vec2(0.5, 0.5);
    vec2 dirToCenter = uv - center;
    float distanceToCenter = length(dirToCenter);

    float atacTotal = atac1 + atac2;
    
    dirToCenter = normalize(dirToCenter);
    vec2 dirNorth = vec2(0.0, 1.0);

    float angle = atan(dirToCenter.y, dirToCenter.x);
    // Adjusting angle from [-PI, PI] to [0, 2*PI] range
    if (angle < 0.0) angle += 2.0 * PI;
    
    // Convert angle to a [0, 1] range
    float normalizedAngle = angle / (2.0 * PI);

    // Determine color based on the normalized angle and segment proportions
    if (normalizedAngle < (atac1 / atacTotal)) {
        color = ruletaColor1;
    } else {
        color = ruletaColor2;
    }
    
    // Apply transparency outside the roulette circle
    if (distanceToCenter > 0.5) {
        alpha = 0.0;
    }

    FragColor = vec4(color, alpha);
}
