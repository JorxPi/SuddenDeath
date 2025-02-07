#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;         // Size of glyph
    glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer {
public:
    std::map<char, Character> Characters;
    unsigned int VAO, VBO;
    unsigned int shaderProgram;
    unsigned int projectionLocation;

    TextRenderer(int width, int height);
    void LoadFont(const std::string& fontPath, unsigned int fontSize);
    void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);
    float CalculateTextWidth(const std::string& text, float scale);

};

#endif
