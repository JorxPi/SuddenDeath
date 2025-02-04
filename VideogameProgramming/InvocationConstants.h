#pragma once
#include <glm/vec2.hpp>
#include <vector>

struct InvocationData {
    const char* texturePath;
    glm::vec2 size;
};

namespace Invocations {
    const InvocationData data[] = {
        { "Textures/inv_Munch.png", glm::vec2(61., 92.) },
        { "Textures/inv_Selly.png", glm::vec2(61., 92.) },
        { "Textures/inv_Fall.png", glm::vec2(88., 63.) },
        { "Textures/inv_Gardol.png", glm::vec2(90., 50.) },
        { "Textures/inv_Kin.png", glm::vec2(88., 75.) },
        { "Textures/inv_Slaim.png", glm::vec2(79., 45.) },
        { "Textures/inv_Torla.png", glm::vec2(79., 99.) },
        { "Textures/inv_Champen.png", glm::vec2(76., 86.) },
        { "Textures/inv_Gobobo.png", glm::vec2(61., 92.) }
    };

    const int numInvocations = sizeof(data) / sizeof(data[0]);
}