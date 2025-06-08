#include "RenderSystem.h"
#include <GLFW/glfw3.h>

RenderSystem::RenderSystem(int width, int height) {
    projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    sr.Init();
    
}

double prevTime = glfwGetTime();

void RenderSystem::tick(World* world, float deltaTime)
{
    t += glfwGetTime() - prevTime;
    prevTime = glfwGetTime();

    sr.SetTime(t);

    std::vector<std::pair<Entity*, int>> entities; // Store entity and its creation order
    static int creationOrderCounter = 0;

    world->each<Sprite>([&](Entity* ent, ComponentHandle<Sprite> sprite) {
        entities.push_back(std::make_pair(ent, creationOrderCounter++));
        });

    // Sort entities by layer and then by creation order
    std::sort(entities.begin(), entities.end(), [](const std::pair<Entity*, int>& a, const std::pair<Entity*, int>& b) {
        auto aLayer = a.first->get<Transform>()->layer;
        auto bLayer = b.first->get<Transform>()->layer;

        if (aLayer == bLayer) {
            return a.second < b.second; // Sort by creation order within the same layer
        }

        return aLayer < bLayer;
        });

    for (const auto& pair : entities) {
        Entity* ent = pair.first;
        ComponentHandle<Sprite> sprite = ent->get<Sprite>();
        ComponentHandle<Transform> transform = ent->get<Transform>();

        Texture texture = textureManager.GetTexture(sprite->filepath);

        if (sprite->visible) {
            if (sprite->shaderName == "ruleta") {
                auto rouletteData = ent->get<RouletteData>();
                if (rouletteData) {
                    sr.DrawSprite(texture, projection, transform->getPosition(), texture.GetSize(), transform->rotation, sprite->color, sprite->shaderName, rouletteData->atac1, rouletteData->atac2, rouletteData->color1, rouletteData->color2);
                }
                else {
                    std::cerr << rouletteData->atac1 << std::endl;
                }
            }
            else if (sprite->shaderName == "menu_motion") {
                sr.DrawSprite(
                    texture,
                    projection,
                    transform->getPosition(),
                    texture.GetSize(), 
                    transform->rotation,
                    sprite->color,
                    sprite->shaderName,

                    // roulette params (use defaults: 0,0,any)
                    0.0f,
                    0.0f,
                    glm::vec3(1, 0, 0),
                    glm::vec3(0, 1, 0),

                    //  motion params 
                    sprite->ampScale,
                    sprite->ampAngle,
                    sprite->freq,
                    sprite->phase
                );
            }
            else {
                if (sprite->autoSize) {
                    sr.DrawSprite(texture, projection, transform->getPosition(), texture.GetSize(), transform->rotation, sprite->color, sprite->shaderName);
                }
                else {
                    sr.DrawSprite(texture, projection, transform->getPosition(), sprite->size, transform->rotation, sprite->color, sprite->shaderName);
                }
            }
        }
    }
}