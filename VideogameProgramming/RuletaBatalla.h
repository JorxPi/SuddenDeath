#pragma once

#include "Ruleta.h"

class Ruleta;

class RuletaBatalla : public Ruleta
{
    using Ruleta::Ruleta;

public:
    //RuletaBatalla(GLFWwindow* window, World* world, Entity* entity);
    //GENERAL LOGIC
    void tickCont() override;
    void determineSpinResult(float finalRotation) override;
    
    double getSpinResult();
    

private:
    //double spinResult;

    bool notDestroyed = true;
};