#pragma once

struct PassiveData {
    const char* name;
    const char* description;
};

namespace PassiveDescriptions {
    constexpr int Count = 16;

    static const PassiveData data[Count] = {
        { "Patient Strength",      "Gain 1 attack every time this invocation doesn't move" },
        { "Territorial Control",  "When an enemy invocation conquers a positive cell, reduce its attack by 2" },
        { "Against All Odds",     "After turn 5, if the player owning this invocation doesn't control any conquerable cell gain 6 points. Can only be activated once." },
        { "Reverse Fate",    "The result of the combats of this invocation is reversed" },
        { "Silent Execution",      "Destroy any enemy summon that remains stationary for 2 consecutive turns" },
        { "Underestimate","If a rival summons an invocation with more attack than this, its attack is reduced by 1" },
        { "Strike Weakness",   "When attacking another summon, reduce its attack by 1" },
        { "Rogue Activator",    "This summon can activate positive spaces belonging to rival players" },
        { "Triumphant Surge",      "If this summon wins a combat, the player gains 2 extra sudden points" },
        { "Mass Demoralize",    "If this summon wins a combat, all rival players lose 1 sudden point" },
        { "Second Chance", "If the player’s first coin flip of the turn is tails, flip the coin again" },
        { "Comeback Duelist",   "If this summon loses a combat, it repeats the combat, but the enemy summon gains 3 attack for the second round." },
        { "Fortune’s Favor",    "Get a positive effect after a tails coinflip" },
        { "Gap Brawler",        "This summon can battle an enemy summon with exactly 2 less attack points regardless of positioning" },
        { "Frontline Leap",   "On this summon’s third turn alive, it can teleport directly into a conquerable cell" },
        { "Compensation Denier",          "If an oponent has more sudden points than the player holding this passive, its tails conflips don't award points" }
    };

    inline const char* GetName(int id) { return data[id].name; }
    inline const char* GetDescription(int id) { return data[id].description; }
}