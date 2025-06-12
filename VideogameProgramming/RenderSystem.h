#pragma once

#include<iostream>
#include<string>
#include<map>
#include "ECS.h"
#include "Components.h"
#include "Texture.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"

using namespace std;
using namespace ECS;

class TextRenderer;

class RenderSystem : public EntitySystem
{
public:

    RenderSystem(int width, int height);
    virtual void tick(World* world, float deltaTime) override;
    void setTextRendererRef(TextRenderer* tr);

private:
    TextureManager textureManager;
    SpriteRenderer sr;
    TextRenderer* textRendererRef;

    glm::mat4 projection;

    float t = 0;
};