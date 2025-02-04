#pragma once
#include <string>
#include <glm/glm.hpp>

struct TextComponent {
    std::string text;
    glm::vec2 position;
    float scale;
    glm::vec3 color;

    TextComponent(std::string t, glm::vec2 pos, float s, glm::vec3 col)
        : text(t), position(pos), scale(s), color(col) {
    }
};
