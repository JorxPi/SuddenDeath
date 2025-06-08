#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D u_Texture;
void main() {
    vec4 c = texture(u_Texture, TexCoords);
    FragColor = vec4(c.rgb * 0.8, c.a);
}