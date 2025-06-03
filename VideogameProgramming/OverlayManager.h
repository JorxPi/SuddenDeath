#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <chrono>   
#include <glm/glm.hpp>

#include "EntityManager.h"

class EntityManager;

class OverlayManager {
public:
    OverlayManager(EntityManager* entityManager);

    void update(float deltaTime);

    void enter();
    void exit();
    void addOverlayEntity(ECS::Entity* entity);
    void removeOverlayEntity(ECS::Entity* entity);

    void activate(bool state);
    bool isActive() const;
    void lockSelection(bool status);  // Lock or unlock selection globally
    bool isSelectionLocked();  // Check if selection is locked

    void addScoreEntity(int playerId, ECS::Entity* entity); // Add score entities
    void removeScoreEntity(int playerId, ECS::Entity* entity);

    void activateScoreOverlay(bool state); // Activate/Deactivate score overlay
    void updateScoreVisibility(int playerId, int score); // Update score visibility

    EntityManager* getEntityManager();

    void clearAllOverlays();
private:
    EntityManager* entityManager;

    std::vector<ECS::Entity*> overlayEntities;
    std::map<int, std::vector<std::pair<ECS::Entity*, bool>>> scoreEntities;

    bool active;
    bool entering = false;
    bool exiting = false;
    bool selectionLocked = false;

    std::chrono::steady_clock::time_point transitionStartTime;
    float transitionDuration = 1.0f;
};