#include "CellConstants.h"

std::unordered_map<glm::vec2, std::vector<int>, Vec2Hash> highlightMap = {
    {CellPos::c1, {1, 4, 10}},
    {CellPos::c2, {0, 2, 3}},
    {CellPos::c3, {1, 6, 12}},
    {CellPos::c4, {1, 4, 6}},
    {CellPos::c5, {0, 3, 5}},
    {CellPos::c6, {4, 6, 8}},
    {CellPos::c7, {2, 3, 5}},
    {CellPos::ha, {1}},
    {CellPos::ta, {5}},
    {CellPos::hr, {6}},
    {CellPos::tr, {0}},
    {CellPos::hg, {4}},
    {CellPos::tg, {2}}
};