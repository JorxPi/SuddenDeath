#include "GameLogManager.h"
#include "TextRenderer.h"

void GameLogManager::AddMessage(const std::vector<LogSegment>& segs) {
    if (messages.size() >= maxMessages)
        messages.erase(messages.begin());
    messages.push_back({ segs });
}

void GameLogManager::RenderLog(TextRenderer* tr, float x, float y, float scale) {
    float lineSpacing = 20.0f;
    float yOffset = 0.0f;

    for (const auto& msg : messages) {
        float cursorX = x;
        for (const auto& seg : msg.segments) {
            tr->RenderText("ActionMan", seg.text, cursorX, y - yOffset, scale, seg.color);
            cursorX += tr->CalculateTextWidth("ActionMan", seg.text, scale);
        }
        yOffset += lineSpacing;
    }
}

void GameLogManager::ClearMessages() {
    messages.clear();
}