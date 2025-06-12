#include "RenderSystem.h"
#include <GLFW/glfw3.h>

RenderSystem::RenderSystem(int width, int height) {
    projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    sr.Init();
    
}

double prevTime = glfwGetTime();

void RenderSystem::tick(World* world, float deltaTime)
{
    // 1) Actualizamos el tiempo y pasamos a SpriteRenderer
    t += glfwGetTime() - prevTime;
    prevTime = glfwGetTime();
    sr.SetTime(t);

    // 2) Recogemos todas las entidades con Sprite y con TextLabel
    std::vector<std::pair<Entity*, int>> entities;
    static int creationOrderCounter = 0;

    world->each<Sprite>([&](Entity* ent, ComponentHandle<Sprite> sprite) {
        entities.emplace_back(ent, creationOrderCounter++);
        });
    world->each<TextLabel>([&](Entity* ent, ComponentHandle<TextLabel> label) {
        entities.emplace_back(ent, creationOrderCounter++);
        });

    // 3) Ordenamos por layer y luego por orden de creación
    std::sort(entities.begin(), entities.end(),
        [&](auto& a, auto& b) {
            int la = a.first->get<Transform>()->layer;
            int lb = b.first->get<Transform>()->layer;
            if (la != lb) return la < lb;
            return a.second < b.second;
        });

    // 4) Recorremos e intercalamos sprites y texto
    for (auto& pr : entities) {
        Entity* ent = pr.first;
        auto   transformC = ent->get<Transform>();          // ComponentHandle<Transform>
        glm::vec2 pos = transformC->getPosition();

        // —— Sprites ——
        if (ent->has<Sprite>()) {
            auto spr = ent->get<Sprite>();
            if (!spr->visible) continue;
            Texture tx = textureManager.GetTexture(spr->filepath);

            if (spr->shaderName == "ruleta") {
                auto rd = ent->get<RouletteData>();
                if (rd) {
                    sr.DrawSprite(tx, projection, pos, tx.GetSize(),
                        transformC->rotation, spr->color,
                        spr->shaderName,
                        rd->atac1, rd->atac2,
                        rd->color1, rd->color2);
                }
            }
            else if (spr->shaderName == "menu_motion") {
                glm::vec2 size = spr->autoSize ? tx.GetSize() : spr->size;
                sr.DrawSprite(tx, projection, pos, size,
                    transformC->rotation, spr->color,
                    spr->shaderName,
                    0, 0, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0),
                    spr->ampScale, spr->ampAngle,
                    spr->freq, spr->phase);
            }
            else {
                glm::vec2 size = spr->autoSize ? tx.GetSize() : spr->size;
                sr.DrawSprite(tx, projection, pos, size,
                    transformC->rotation, spr->color,
                    spr->shaderName);
            }
            continue;
        }

        // —— TextLabels ——
        if (ent->has<TextLabel>()) {
            auto lbl = ent->get<TextLabel>();
            float textScale = transformC->scale;
            // centrar X    
            float w = textRendererRef->CalculateTextWidth(
                lbl->fontName, lbl->text, textScale);
            float x_text = pos.x - w * 0.5f;
            // convertir Y a sistema TextRenderer (0 abajo)
            float y_text = 800 - pos.y;
            textRendererRef->RenderText(
                lbl->fontName,
                lbl->text,
                x_text,
                y_text,
                textScale,
                lbl->color
            );
        }
    }
}

void RenderSystem::setTextRendererRef(TextRenderer* tr) {
    textRendererRef = tr;
}
