#pragma once

#include "Ruleta.h"

class Ruleta;

class RuletaAttacks : public Ruleta
{
    using Ruleta::Ruleta;

public:
    //RuletaAttacks(GLFWwindow* window, World* world, Entity* entity);
    //GENERAL LOGIC
    void tickCont() override;
    void determineSpinResult(float finalRotation) override;

    double getSpinResult();

private:
    //double spinResult;

    bool invocated = false;
};