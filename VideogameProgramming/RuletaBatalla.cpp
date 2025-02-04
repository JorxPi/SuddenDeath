#include "RuletaBatalla.h"
#include <cmath>
#include <random>
#include <chrono>

/*RuletaBatalla::RuletaBatalla(GLFWwindow* window, World* world, Entity* entity)
    : Ruleta(window, world, entity)  {

}*/

void RuletaBatalla::tickCont(){
    if (rotationEnded && !checkingDuration) {
        roulTime = std::chrono::steady_clock::now();
        checkingDuration = true;
    }

    if (checkingDuration) {
        auto currentTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - roulTime).count();

        if (duration >= 3 && notDestroyed) {
            notDestroyed = false;
            //gameRef->destroyRuletaBatalla();
            readyToDestroy = true;
        }
    }
}

void RuletaBatalla::determineSpinResult(float finalRotation){
    auto rouletteData = entity->get<RouletteData>();

    double normalizedRotation = 360.0 - finalRotation;

    double totalAtac = rouletteData->atac1 + rouletteData->atac2;
    double atac1SegmentSize = 360.0 * (rouletteData->atac1 / totalAtac);
    double atac1StartAngle = 90.0;
    double atac1EndAngle = atac1StartAngle - atac1SegmentSize;
    if (atac1EndAngle < 0) atac1EndAngle += 360.0;

    if ((normalizedRotation < atac1StartAngle && normalizedRotation > atac1EndAngle) ||
        (atac1EndAngle > atac1StartAngle && (normalizedRotation < atac1StartAngle || normalizedRotation > atac1EndAngle))) {
        spinResult = 1;
    }
    else {
        spinResult = 2;
    }

    rotationEnded = true;
}


double RuletaBatalla::getSpinResult() {
    return spinResult;
}






