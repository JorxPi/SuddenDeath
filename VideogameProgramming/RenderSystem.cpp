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

    // All Entities of the world are stored
    std::vector<std::pair<Entity*, int>> entities;
    static int creationOrderCounter = 0;

    world->each<Sprite>([&](Entity* ent, ComponentHandle<Sprite> sprite) {
        entities.emplace_back(ent, creationOrderCounter++);
        });
    world->each<TextLabel>([&](Entity* ent, ComponentHandle<TextLabel> label) {
        entities.emplace_back(ent, creationOrderCounter++);
        });

    // They are ordered by layer and after by cration order
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
            glm::vec2 size = spr->autoSize ? tx.GetSize() : spr->size;

            // prepara datos de ruleta (sólo válidos si shaderName=="ruleta")
            float        atac1 = 0.0f, atac2 = 0.0f;
            glm::vec3 color1(1.0f), color2(0.0f);
            if (spr->shaderName == "ruleta") {
                auto rd = ent->get<RouletteData>();
                if (rd) {
                    atac1 = rd->atac1;
                    atac2 = rd->atac2;
                    color1 = rd->color1;
                    color2 = rd->color2;
                }
            }

            sr.DrawSprite(
                tx, projection,
                pos, size,
                transformC->rotation,
                spr->color,
                spr->shaderName,

                atac1, atac2,
                color1, color2,

                spr->ampScale,
                spr->ampAngle,
                spr->freq,
                spr->phase
            );

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
