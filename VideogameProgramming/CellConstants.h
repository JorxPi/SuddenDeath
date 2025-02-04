#pragma once
#include <glm/vec2.hpp>
#include <unordered_map>
#include <vector>
#include <functional>

namespace CellPos {
    constexpr glm::vec2 c1 = { 380.f, 260.f };
    constexpr glm::vec2 c2 = { 580.f, 200.f };
    constexpr glm::vec2 c3 = { 800.f, 300.f };
    constexpr glm::vec2 c4 = { 600.f, 400.f };
    constexpr glm::vec2 c5 = { 400.f, 490.f };
    constexpr glm::vec2 c6 = { 640.f, 600.f };
    constexpr glm::vec2 c7 = { 840.f, 520.f };
    constexpr glm::vec2 ha = { 670.f, 110.f };
    constexpr glm::vec2 ta = { 720.f, 680.f };
    constexpr glm::vec2 hr = { 950.f, 600.f };
    constexpr glm::vec2 tr = { 340.f, 160.f };
    constexpr glm::vec2 hg = { 300.f, 570.f };
    constexpr glm::vec2 tg = { 890.f, 350.f };
}


//NECESARIO PARA MAP DE glm::vec2
struct Vec2Hash {
    size_t operator()(const glm::vec2& vec) const {
        std::hash<float> hasher;
        size_t h1 = hasher(vec.x);
        size_t h2 = hasher(vec.y);
        return h1 ^ (h2 << 1); // Combine the hash of x and y
    }
};

extern std::unordered_map<glm::vec2, std::vector<int>, Vec2Hash> highlightMap;
