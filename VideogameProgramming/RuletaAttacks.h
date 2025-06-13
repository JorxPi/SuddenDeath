#pragma once

#include "Ruleta.h"

class Ruleta;

class RuletaAttacks : public Ruleta
{
    using Ruleta::Ruleta;

public:
    //RuletaAttacks(GLFWwindow* window, World* world, Entity* entity);
    //GENERAL LOGIC
    void tickScript(float deltaTime) override;
    void tickCont() override;
    void determineSpinResult(float finalRotation) override;

    double getSpinResult();

    void setFlechasEntity(ECS::Entity* flechas);

private:
    //double spinResult;

    bool invocated = false;

    ECS::Entity* flechasEnt = nullptr;
    bool         destroyedFlechas = false;
};