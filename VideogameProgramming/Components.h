#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

struct Transform
{
    Transform(glm::vec2 position, float rotation, float scale, int layer = 1) {
        this->position = position;
        this->lPos = position;
        this->tPos = position;
        this->rotation = rotation;
        this->scale = scale;
        this->layer = layer;
        this->parent = nullptr;
    }

    glm::vec2 position;
    glm::vec2 lPos;
    glm::vec2 tPos;
    float rotation;
    float scale;
    int layer;
    Transform* parent;

    void assignParent(Transform* newParent) {
        parent = newParent;
    }

public:
    glm::vec2 getPosition() {
        if (!parent) {
            return position;
        }
        return position + parent->getPosition();
    }
};

struct Sprite
{
    Sprite(const char* filepath, glm::vec3 color, bool autoSize = true, glm::vec2 size = glm::vec2(1, 1), const char* shaderName = "default", bool visible = true) {
        this->filepath = filepath;
        this->color = color;
        this->autoSize = autoSize;
        this->size = size;
        this->shaderName = shaderName;
        this->visible = visible;
    }

    const char* filepath;
    glm::vec3 color;
    bool autoSize;
    glm::vec2 size = glm::vec2(1, 1);
    const char* shaderName;
    bool visible;

    //Shader variables
    float       ampScale = 0.00f;
    float       ampAngle = 0.00f;
    float       freq = 1.0f;
    float       phase = 0.0f;
};

struct BoxCollider
{
    BoxCollider(float w, float h, int colId = 0) {
        this->width = w;
        this->height = h;
        this->colliderId = colId;
    }

    float width = 0;
    float height = 0;
    bool collidedWith = false;
    int colliderId = 0;
};

struct ScriptComponent
{
    ScriptComponent(int scriptId) {
        this->scriptId = scriptId;
    }

    int scriptId = -1;
};

struct RouletteData {
    RouletteData(glm::vec3 color1 = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 color2 = glm::vec3(0.0f, 1.0f, 0.0f), float atac1 = 0.0f, float atac2 = 0.0f) {
        this->color1 = color1;
        this->color2 = color2;
        this->atac1 = atac1;
        this->atac2 = atac2;
    }

    glm::vec3 color1;
    glm::vec3 color2;
    float atac1;
    float atac2;
};

struct TextLabel {
    TextLabel(const std::string& _text,const std::string& _fontName, const glm::vec3& _color) {
        this->text = _text;
        this->fontName = _fontName;
        this->color = _color;
    }

    std::string text;
    std::string fontName;
    glm::vec3   color;

};