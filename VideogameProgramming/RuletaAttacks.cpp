#include "RuletaAttacks.h"
#include <chrono>   
#include <cmath>
#include <random>

/*RuletaAttacks::RuletaAttacks(GLFWwindow* window, World* world, Entity* entity)
    : Ruleta(window, world, entity) {

}*/



//GENERAL LOGIC



void RuletaAttacks::tickCont() {
    if (rotationEnded && !invocated && !isSpinning) {
        roulTime = std::chrono::steady_clock::now();
        invocated = true;
        checkingDuration = true;
    }

    if (checkingDuration) {
        auto currentTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - roulTime).count();

        if (duration >= 3) {
            readyToDestroy = true;
        }
    }
}

void RuletaAttacks::determineSpinResult(float finalRotation) {
    int sector = static_cast<int>(finalRotation / 60);
    spinResult = 6 - sector;
    rotationEnded = true;
    std::cout << "Rotation: " << finalRotation << std::endl;
    std::cout << "Spin Complete. Landing Result: " << spinResult << std::endl;
}

double RuletaAttacks::getSpinResult() {
    return spinResult;
}