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

    // Show a named overlay group (if it was hidden, animate it in)
    void showOverlayGroup(const std::string& groupName);

    // Hide a named overlay group (animate it out)
    void hideOverlayGroup(const std::string& groupName);

    // Toggle visibility: if shown, hide; if hidden, show
    void toggleOverlayGroup(const std::string& groupName);

    glm::vec2 getOverlayOffset(const std::string& groupName) const;

    // Register a new entity into a specific overlay group.
    // It will be positioned offscreen initially (based on your logic) and moved in when showOverlayGroup is called.
    void addOverlayEntity(const std::string& groupName, ECS::Entity* entity);

    // If you need to remove an entity from a specific group at runtime:
    void removeOverlayEntity(const std::string& groupName, ECS::Entity* entity);

    void lockSelection(const std::string& groupName, bool status);
    bool isSelectionLocked(const std::string& groupName) const;

    bool isOverlayGroupTransitioning(const std::string& groupName) const;


    void addScoreEntity(int playerId, ECS::Entity* entity); // Add score entities
    void removeScoreEntity(int playerId, ECS::Entity* entity);

    void updateScoreVisibility(int playerId, int score); // Update score visibility

    EntityManager* getEntityManager();

    void clearAllOverlays();

    void removeEntityFromAllGroups(ECS::Entity* ent);

private:
    struct OverlayGroup {
        std::vector<ECS::Entity*> entities;
        bool active = false;      // true if currently fully shown
        bool entering = false;      // true if in the process of animating in
        bool exiting = false;      // true if in the process of animating out
        bool selectionLocked = false; // if true, ignore input within this group

        std::chrono::steady_clock::time_point transitionStartTime;

    };

    EntityManager* entityManager;


    std::map<std::string, OverlayGroup> groups;

    std::map<int, std::vector<std::pair<ECS::Entity*, bool>>> scoreEntities;

    float transitionDuration = 1.0f;

    // Helper to move entities in a group to their tPos based on t  [0,1]
    void applyTransition(const std::string& name, float t);
};