#pragma once
#include <glm/vec2.hpp>
#include <vector>

struct IdleParams {
    float ampScale;   // ± escala
    float ampAngle;   // ± ángulo en radianes
    float freq;       // ciclos por segundo
    float phase;      // desfase en radianes
};

struct InvocationData {
    const char* name;
    const char* texturePath;
    glm::vec2 size;
    IdleParams  idle;
};

namespace Invocations {
    const InvocationData data[] = {
        { "Munch", "Textures/1.png", glm::vec2(65.f, 105.f), 0.05f, glm::radians(1.0f), 1.0f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>()},
        { "Selly", "Textures/2.png", glm::vec2(82.f, 100.f), 0.10f, glm::radians(2.0f), 1.5f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>()},
        { "Fall", "Textures/3.png", glm::vec2(68.f, 102.f), 0.08f, glm::radians(3.0f), 0.8f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>()},
        { "Gardol", "Textures/4.png", glm::vec2(79.f, 104.f), 0.03f, glm::radians(2.5f), 0.8f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Kin", "Textures/5.png",   glm::vec2(79.f, 100.f),  0.06f, glm::radians(3.0f), 1.8f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Slaim", "Textures/6.png", glm::vec2(79.f, 100.f), 0.15f, 0.0f, 2.5f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Torla", "Textures/7.png", glm::vec2(82.f, 100.f), 0.10f, glm::radians(1.5f), 1.2f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Champen", "Textures/8.png", glm::vec2(58.f, 108.f), 0.05f, glm::radians(1.5f), 1.2f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Gobobo", "Textures/11.png", glm::vec2(65.f, 104.f), 0.05f, glm::radians(2.0f), 1.0f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Kiut", "Textures/9.png", glm::vec2(50.f, 101.f), 0.07f, glm::radians(2.0f), 2.0f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Greg", "Textures/10.png", glm::vec2(50.f, 101.f), 0.04f, glm::radians(3.0f), 2.5f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Wagol", "Textures/inv_Wagol.png", glm::vec2(61.f, 92.f), 0.07f, glm::radians(2.0f), 0.6f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>()}
    };

    const int numInvocations = sizeof(data) / sizeof(data[0]);
}