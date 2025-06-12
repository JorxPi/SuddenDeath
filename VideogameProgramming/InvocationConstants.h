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
        { "Munch", "Textures/inv_Munch.png", glm::vec2(61.f, 92.f), 0.05f, glm::radians(1.0f), 1.0f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>()},
        { "Selly", "Textures/inv_Selly.png", glm::vec2(61.f, 92.f), 0.10f, glm::radians(2.0f), 3.0f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>()},
        { "Fall", "Textures/inv_Fall.png", glm::vec2(88.f, 63.f), 0.08f, glm::radians(3.0f), 0.8f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>()},
        { "Gardol", "Textures/inv_Gardol.png", glm::vec2(90.f, 50.f), 0.03f, glm::radians(2.5f), 0.8f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Kin", "Textures/inv_Kin.png",   glm::vec2(88.f, 75.f),  0.06f, glm::radians(3.0f), 1.8f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Slaim", "Textures/inv_Slaim.png", glm::vec2(79.f, 45.f), 0.15f, 0.0f, 2.5f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Torla", "Textures/inv_Torla.png", glm::vec2(79.f, 99.f), 0.10f, glm::radians(1.5f), 1.2f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Champen", "Textures/inv_Champen.png", glm::vec2(76.f, 86.f), 0.05f, glm::radians(1.5f), 1.2f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Gobobo", "Textures/inv_Gobobo.png", glm::vec2(61.f, 92.f), 0.05f, glm::radians(2.0f), 1.0f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Kiut", "Textures/inv_Kiut.png", glm::vec2(75.f, 92.f), 0.12f, glm::radians(4.0f), 2.0f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Greg", "Textures/inv_Greg.png", glm::vec2(80.f, 92.f), 0.04f, glm::radians(3.0f), 2.5f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>() },
        { "Wagol", "Textures/inv_Wagol.png", glm::vec2(61., 92.), 0.07f, glm::radians(2.0f), 0.6f, static_cast<float>(rand()) / RAND_MAX * 2.f * glm::pi<float>()}
    };

    const int numInvocations = sizeof(data) / sizeof(data[0]);
}