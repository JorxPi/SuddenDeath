#include "OverlaySystem.h"
#include "StartGameScreen.h"

OverlaySystem::OverlaySystem(EntityManager* entityManager)
    :entityManager(entityManager) {}

void OverlaySystem::tick(ECS::World* world, float dtTicks) {
    float dt = dtTicks / float(CLOCKS_PER_SEC);
    for (auto& kv : groups) {
        const std::string& name = kv.first;
        OverlayGroup& group = kv.second;

        if (!group.entering && !group.exiting)
            continue;

        // 1) Accumulate delta-time
        group.elapsed += dt;

        // 2) Compute normalized transition factor
        float t = std::min(group.elapsed / transitionDuration, 1.0f);

        // 3) Apply the transition at time t
        applyTransition(name, t);

        // 4) Once complete, update flags and reset elapsed timer
        if (t >= 1.0f) {
            if (group.entering)  group.active = true;
            // leaving or any end-of-transition state
            group.entering = false;
            group.exiting = false;
            group.elapsed = 0.0f;
        }
    }
}

void OverlaySystem::addOverlayEntity(const std::string& groupName, ECS::Entity* entity) {
    if (groups.find(groupName) == groups.end()) {
        groups[groupName] = OverlayGroup{};
    }
    groups[groupName].entities.push_back(entity);
}

void OverlaySystem::removeOverlayEntity(const std::string& groupName, ECS::Entity* entity) {
    auto it = groups.find(groupName);
    if (it == groups.end()) return;
    OverlayGroup& group = it->second;
    auto& vec = group.entities;
    vec.erase(std::remove(vec.begin(), vec.end(), entity), vec.end());
}

void OverlaySystem::showOverlayGroup(const std::string& groupName) {
    auto it = groups.find(groupName);
    if (it == groups.end()) return;
    auto& group = it->second;
    if (group.active || group.entering) return;

    group.entering = true;
    group.exiting = false;
    group.active = false;
    group.elapsed = 0.0f;

    glm::vec2 offset;
    if (groupName == INVOCATIONS_OVERLAY) {
        offset = glm::vec2(-820.f, 0.f);
    }
    else if (groupName == SCORE_BLUE_OVERLAY ||
        groupName == SCORE_RED_OVERLAY ||
        groupName == SCORE_GREEN_OVERLAY) {
        offset = glm::vec2(0.f, +300.f);
    }
    else if (groupName == INFO_OVERLAY) {
        offset = glm::vec2(0.f, -145.f);
    }
    else if (groupName == LOG_OVERLAY) {
        offset = glm::vec2(+460.f, 0.f);
    }
    else {
        offset = glm::vec2(+820.f, 0.f);
    }

    for (auto* e : group.entities) {
        auto t = e->get<Transform>();
        t->lPos = t->position;           // start where it is now (off-screen)
        t->tPos = t->position + offset;  
    }
}

void OverlaySystem::hideOverlayGroup(const std::string& groupName) {
    auto it = groups.find(groupName);
    if (it == groups.end()) return;
    auto& group = it->second;
    if (!group.active || group.exiting) return;

    group.exiting = true;
    group.entering = false;
    group.active = false;
    group.elapsed = 0.0f;

    glm::vec2 offset;
    if (groupName == INVOCATIONS_OVERLAY) {
        offset = glm::vec2(-820.f, 0.f);
    }
    else if (groupName == SCORE_BLUE_OVERLAY ||
        groupName == SCORE_RED_OVERLAY ||
        groupName == SCORE_GREEN_OVERLAY) {
        offset = glm::vec2(0.f, +300.f);
    }
    else if (groupName == INFO_OVERLAY) {
        offset = glm::vec2(0.f, -145.f);
    }
    else if (groupName == LOG_OVERLAY) {
        offset = glm::vec2(+460.f, 0.f);
    }
    else {
        offset = glm::vec2(+820.f, 0.f);
    }

    for (auto* e : group.entities) {
        auto t = e->get<Transform>();
        t->lPos = t->position;            // start on-screen
        t->tPos = t->position - offset;   // move off-screen again
    }
}

void OverlaySystem::toggleOverlayGroup(const std::string& groupName) {
    auto it = groups.find(groupName);
    if (it == groups.end()) return;

    OverlayGroup& group = it->second;

    if (group.entering || group.exiting) {
        return;
    }

    if (group.active) {
        hideOverlayGroup(groupName);
    }
    else {
        showOverlayGroup(groupName);
    }
}

glm::vec2 OverlaySystem::getOverlayOffset(const std::string& groupName) const {
    auto it = groups.find(groupName);
    if (it == groups.end()) return glm::vec2(0.f);

    const auto& group = it->second;
    // Repite el cálculo de rawOffset tal cual lo haces en show/hide
    glm::vec2 rawOffset;
    if (groupName == LOG_OVERLAY)         rawOffset = glm::vec2(+460.f, 0.f);
    else if (groupName == INVOCATIONS_OVERLAY) rawOffset = glm::vec2(-820.f, 0.f);
    else if (groupName == INFO_OVERLAY)    rawOffset = glm::vec2(0.f, -145.f);
    else /*otros*/                        rawOffset = glm::vec2(+820.f, 0.f);

    // Suponiendo que tu transición dura `transitionDuration` segundos
    float t = glm::clamp(group.elapsed / transitionDuration, 0.f, 1.f);

    if (group.entering)  return rawOffset * t;          
    if (group.exiting)   return rawOffset * (1.f - t);  
    if (group.active)    return rawOffset;              
    return glm::vec2(0.f);
}

void OverlaySystem::applyTransition(const std::string& name, float t) {
    auto& group = groups[name];
    for (ECS::Entity* e : group.entities) {
        auto tf = e->get<Transform>();
        tf->position.x = tf->lPos.x + t * (tf->tPos.x - tf->lPos.x);
        tf->position.y = tf->lPos.y + t * (tf->tPos.y - tf->lPos.y);
    }
}

void OverlaySystem::lockSelection(const std::string& groupName, bool status) {
    if (groups.find(groupName) == groups.end()) return;
    groups[groupName].selectionLocked = status;
}

bool OverlaySystem::isSelectionLocked(const std::string& groupName) const {
    auto it = groups.find(groupName);
    return (it == groups.end()) ? false : it->second.selectionLocked;
}

bool OverlaySystem::isOverlayGroupTransitioning(const std::string& groupName) const {
    auto it = groups.find(groupName);
    if (it == groups.end()) return false;
    const OverlayGroup& g = it->second;
    return g.entering || g.exiting;
}

// SCORE


void OverlaySystem::addScoreEntity(int playerId, ECS::Entity* entity) {
    scoreEntities[playerId].emplace_back(entity, false); 
}

void OverlaySystem::removeScoreEntity(int playerId, ECS::Entity* entity) {
    auto mapIt = scoreEntities.find(playerId);
    if (mapIt == scoreEntities.end()) {
        return;
    }

    auto& vec = mapIt->second;
    vec.erase(std::remove_if(
        vec.begin(),
        vec.end(),
        [&](const std::pair<ECS::Entity*, bool>& p) {
            return p.first == entity;
        }),
        vec.end());

    if (vec.empty()) {
        scoreEntities.erase(mapIt);
    }
}

void OverlaySystem::updateScoreVisibility(int playerId, int score) {
    auto it = scoreEntities.find(playerId);
    if (it == scoreEntities.end()) return;

    auto& playerPoints = it->second;
    for (int i = 0; i < (int)playerPoints.size(); i++) {
        bool visible = (i < score);
        playerPoints[i].second = visible;  // update your internal flag

        // now actually show/hide the sprite
        if (auto sprite = playerPoints[i].first->get<Sprite>()) {
            sprite->visible = visible;
        }
    }
}

EntityManager* OverlaySystem::getEntityManager() {
    return entityManager;
}

void OverlaySystem::clearAllOverlays() {
    for (auto& kv : groups) {
        for (ECS::Entity* e : kv.second.entities) {
            if (e) {
                entityManager->destroyEntity(e);
            }
        }
    }
    groups.clear();

    for (auto& kv : scoreEntities) {
        for (auto& pairEnt : kv.second) {
            ECS::Entity* e = pairEnt.first;
            if (e) {
                entityManager->destroyEntity(e);
            }
        }
    }
    scoreEntities.clear();
}

void OverlaySystem::removeEntityFromAllGroups(ECS::Entity* ent) {
    for (auto& kv : groups) {
        const std::string& groupName = kv.first;
        removeOverlayEntity(groupName, ent);
    }
}