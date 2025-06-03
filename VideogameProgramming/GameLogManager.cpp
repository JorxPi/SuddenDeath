#include "GameLogManager.h"
#include "TextRenderer.h"

void GameLogManager::AddMessage(const std::string& text, glm::vec3 color) {
    // If the log is full, remove the oldest message
    if (messages.size() >= maxMessages) {
        messages.erase(messages.begin());
    }

    // Add new message
    messages.push_back({ text, color });
}

void GameLogManager::RenderLog(TextRenderer* textRenderer, float x, float y, float scale) {
    float lineSpacing = 20.0f;  // Spacing between lines
    float yOffset = 0.0f;

    for (const auto& message : messages) {
        textRenderer->RenderText("ActionMan", message.text, x, y - yOffset, scale, message.color);
        yOffset += lineSpacing;
    }
}

void GameLogManager::ClearMessages() {
    messages.clear();
}