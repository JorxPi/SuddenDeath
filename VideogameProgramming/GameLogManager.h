#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct LogSegment {
    std::string text;
    glm::vec3 color;
};

struct LogMessage {
    std::vector<LogSegment> segments;
};

class GameLogManager {
public:
    void AddMessage(const std::vector<LogSegment>& segs);
    void RenderLog(class TextRenderer* textRenderer, float x, float y, float scale);
    void ClearMessages();

private:
    std::vector<LogMessage> messages;
    const int maxMessages = 6;  
};
