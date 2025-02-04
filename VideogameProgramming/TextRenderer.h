#pragma once

#include <map>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>  
#include <ft2build.h>
#include FT_FREETYPE_H

// Character struct to hold OpenGL texture and font metrics
struct Character {
    GLuint TextureID;  // Glyph texture
    glm::ivec2 Size;   // Size of the glyph
    glm::ivec2 Bearing; // Offset from baseline
    GLuint Advance;    // Offset to next glyph
};

// Text rendering class
class TextRenderer {
public:
    TextRenderer();  
    void Init(const std::string& fontPath, int fontSize);  

    void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);

private:
    std::map<char, Character> Characters;
    GLuint VAO, VBO;
    GLuint shaderProgram;
};
