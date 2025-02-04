#pragma once

#include "Script.h"
#include "GameManager.h"
#include <chrono>


class GameManager;

class CoinScript : public Script
{
    using Script::Script;

public:
    void tickScript(float deltaTime) override;

    //GENERAL LOGIC
    void coinFlip();

    //GETTERS
    bool getCoinFlipped();
    bool getHeadOrTails();
    std::chrono::time_point<std::chrono::steady_clock> getFlipTime();

    //SETTERS
    void setGameManagerRef(GameManager* gmr);



private:
    GameManager* gameRef;
    bool coinFlipped = false;
    //Heads = true, Tails = false
    bool headOrTails;
    std::chrono::steady_clock::time_point flipTime;

};
