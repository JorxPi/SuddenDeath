#pragma once
#include "TextRenderer.h"
#include "TextComponent.h"
#include "ECS.h"

class TextSystem : public ECS::EntitySystem {
public:
    TextRenderer* textRenderer;

    TextSystem(TextRenderer* renderer) : textRenderer(renderer) {}

    void tick(ECS::World* world, float deltaTime) override {
        for (auto entity : world->each<TextComponent>()) {
            auto text = entity->get<TextComponent>();  // Fix: Use pointer syntax

            if (text) {  // Check if the entity has the component
                textRenderer->RenderText(
                    text->text,
                    text->position.x,
                    text->position.y,
                    text->scale,
                    text->color
                );
            }
        }
    }
};
