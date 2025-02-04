#pragma once

#include<iostream>
#include<string>
#include<map>
#include "ECS.h"
#include "Components.h"
#include "Texture.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "TextComponent.h"
#include "TextRenderer.h"

using namespace std;
using namespace ECS;

class RenderSystem : public EntitySystem
{
public:

    RenderSystem(int width, int height);
    /*void InitTextRenderer(const std::string& font, int size);
    void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);*/
    virtual void tick(World* world, float deltaTime) override;

    //TextRenderer textRenderer;

private:
    TextureManager textureManager;
    SpriteRenderer sr;

    glm::mat4 projection;

    float t = 0;
};