#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct LogMessage {
    std::string text;
    glm::vec3 color;  
};

class GameLogManager {
public:
    void AddMessage(const std::string& text, glm::vec3 color);
    void RenderLog(class TextRenderer* textRenderer, float x, float y, float scale);

private:
    std::vector<LogMessage> messages;
    const int maxMessages = 6;  
};
