#include "Ruleta.h"
#include <chrono>   
#include <cmath>
#include <random>

/*Ruleta::Ruleta(GLFWwindow* window, World* world, Entity* entity)
    : Script(window, world, entity) {
    
}*/



//GENERAL LOGIC



void Ruleta::tickScript(float deltaTime) {
    ComponentHandle<Transform> transform = entity->get<Transform>();
    ComponentHandle<BoxCollider> collider = entity->get<BoxCollider>();
    ComponentHandle<Sprite> sprite = entity->get<Sprite>();

    if (!isSpinning) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            if (mouseX >= transform->position.x - collider->width / 2.0f && mouseX <= transform->position.x + collider->width / 2.0f &&
                mouseY >= transform->position.y - collider->height / 2.0f && mouseY <= transform->position.y + collider->height / 2.0f) {
                //if (collider->colliderId == gameRef->getCurrentPlayerTurn()) {
                    isSpinning = true;
                    spinStartTime = glfwGetTime();
                    std::random_device rd;
                    std::default_random_engine generator(rd());
                    std::uniform_real_distribution<double> distribution(0.0, 360.0);
                    
                    int fullSpins = 7;
                    double randomFinalAngle = distribution(generator);
                    targetAngle = fullSpins * 360 + randomFinalAngle;
                //}
            }
        }
    }

    if (isSpinning && !rotationEnded) {
     applyRotation(deltaTime);
    }

    tickCont();
}

void Ruleta::applyRotation(float deltaTime) {
    if (!isSpinning) return;

    double elapsedTime = glfwGetTime() - spinStartTime;

    double fractionOfCompletion = elapsedTime / spinDuration;
    fractionOfCompletion = (fractionOfCompletion > 1.0) ? 1.0 : fractionOfCompletion; 
    double easeOutCubic = 1 - pow(1 - fractionOfCompletion, 3);

    ComponentHandle<Transform> transform = entity->get<Transform>();
    double currentRotation = initialRotation + easeOutCubic * (targetAngle - initialRotation);

    transform->rotation = fmod(currentRotation, 360.0);
    if (transform->rotation < 0)
        transform->rotation += 360.0;

    if (elapsedTime >= spinDuration) {
        std::cout << "in 10!" << std::endl;
        transform->rotation = fmod(targetAngle, 360.0); 
        determineSpinResult(transform->rotation);
        isSpinning = false;
        std::cout << "in 11!" << std::endl;
    }
}


//GETTERS



bool Ruleta::getReadyToDestroy() {
    return readyToDestroy;
}



//SETTERS



/*void Ruleta::setGameManagerRef(GameManager* gmr) {
    gameRef = gmr;
}*/
