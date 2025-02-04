#pragma once

#include "Script.h"
#include <chrono>

using namespace std;

//class GameManager;

class Ruleta : public Script
{

    using Script::Script;

public:
    //Ruleta(GLFWwindow* window, World* world, Entity* entity);

    //GENERAL LOGIC
    virtual void tickScript(float deltaTime) override;
    virtual void applyRotation(float deltaTime);
    virtual void tickCont() = 0;
    virtual void determineSpinResult(float finalRotation) = 0;

    //GETTERS
    bool getReadyToDestroy();

    //SETTERS
    //virtual void setGameManagerRef(GameManager* gmr);

protected:
    //GameManager* gameRef;
    bool isSpinning = false;

    double targetAngle;
    double spinStartTime = 0.0;
    double spinDuration = 8.0;
    double initialRotation;
    double spinResult;

    bool rotationEnded = false;
    bool checkingDuration = false;
    bool readyToDestroy = false;

    std::chrono::steady_clock::time_point roulTime;
};