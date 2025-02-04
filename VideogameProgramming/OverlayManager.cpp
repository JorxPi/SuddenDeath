#include "OverlayManager.h"

OverlayManager::OverlayManager(EntityManager* entityManager)
    :entityManager(entityManager) {}

void OverlayManager::update(float deltaTime) {
    if (!entering && !exiting) return;

    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - transitionStartTime).count() / 1000.0f;

    float t = std::min(elapsed / transitionDuration, 1.0f); // Clamp t to [0, 1]

    for (int i = 0; i < overlayEntities.size(); i++) {
        ComponentHandle<Transform> transform = overlayEntities[i]->get<Transform>();
        transform->position.x = transform->lPos.x + t * (transform->tPos.x - transform->lPos.x);
    }

    if (t >= 1.0f) {
        entering = false;
        exiting = false;
    }
}

void OverlayManager::enter() {
    //Move entities inside the screen

    for (int i = 0; i < overlayEntities.size(); i++) {
        ComponentHandle<Transform> transform = overlayEntities[i]->get<Transform>();
        transform->lPos = transform->position;
        transform->tPos = glm::vec2(transform->position.x - 1500, transform->position.y);
    }

    entering = true;
    exiting = false;
    transitionStartTime = std::chrono::steady_clock::now();
    active = true;
}

void OverlayManager::exit() {
    // Move entities outside screen

    for (int i = 0; i < overlayEntities.size(); i++) {
        ComponentHandle<Transform> transform = overlayEntities[i]->get<Transform>();
        transform->lPos = transform->position;
        transform->tPos = glm::vec2(transform->position.x + 1500, transform->position.y);
    }

    entering = false;
    exiting = true;
    transitionStartTime = std::chrono::steady_clock::now();
    active = false;
}

void OverlayManager::addOverlayEntity(ECS::Entity* entity) {
    overlayEntities.push_back(entity);
}

void OverlayManager::activate(bool state) {
    if (state) {
        enter();
        lockSelection(false);
    }
    else {
        exit();
    }
}

bool OverlayManager::isActive() const {
    return active;
}

void OverlayManager::lockSelection(bool status) {
    selectionLocked = status;
}

bool OverlayManager::isSelectionLocked() {
    return selectionLocked;
}


// SCORE


void OverlayManager::addScoreEntity(int playerId, ECS::Entity* entity) {
    scoreEntities[playerId].emplace_back(entity, false); // Add entity with `false` visibility
}

void OverlayManager::activateScoreOverlay(bool state) {
    std::cout << "in 0" << std::endl;
    for (auto& playerEntry : scoreEntities) { // Loop through all players
        std::cout << "in 1" << std::endl;
        for (auto& point : playerEntry.second) { // Loop through each player's points
            auto sprite = point.first->get<Sprite>();
            std::cout << "in 2" << std::endl;
            if (sprite) {
                sprite->visible = (state && (playerEntry.first == -1 || point.second)); // Show if `state` is true and `point.second` is true
                std::cout << "in 3" << std::endl;
            }
        }
    }
}

void OverlayManager::updateScoreVisibility(int playerId, int score) {
    if (scoreEntities.find(playerId) != scoreEntities.end()) {
        auto& playerPoints = scoreEntities[playerId];
        for (int i = 0; i < playerPoints.size(); i++) {
            playerPoints[i].second = (i < score); // Set `true` for points within the score
        }
    }
}