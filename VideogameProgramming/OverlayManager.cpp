#include "OverlayManager.h"
#include "StartGameScreen.h"

OverlayManager::OverlayManager(EntityManager* entityManager)
    :entityManager(entityManager) {}

void OverlayManager::update(float deltaTime) {
    auto now = std::chrono::steady_clock::now();
    for (auto& kv : groups) {
        const std::string& name = kv.first;
        OverlayGroup& group = kv.second;

        if (!group.entering && !group.exiting)
            continue;

        bool wasEntering = group.entering;
        bool wasExiting = group.exiting;

        float elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - group.transitionStartTime)
            .count() / 1000.0f;
        float t = std::min(elapsed / transitionDuration, 1.0f);

        applyTransition(name, t);

        if (t >= 1.0f) {
            if (wasEntering) {
                group.active = true;
            }

            group.entering = false;
            group.exiting = false;
        }
    }
}

void OverlayManager::addOverlayEntity(const std::string& groupName, ECS::Entity* entity) {
    if (groups.find(groupName) == groups.end()) {
        groups[groupName] = OverlayGroup{};
    }
    groups[groupName].entities.push_back(entity);
}

void OverlayManager::removeOverlayEntity(const std::string& groupName, ECS::Entity* entity) {
    auto it = groups.find(groupName);
    if (it == groups.end()) return;
    OverlayGroup& group = it->second;
    auto& vec = group.entities;
    vec.erase(std::remove(vec.begin(), vec.end(), entity), vec.end());
}

void OverlayManager::showOverlayGroup(const std::string& groupName) {
    auto it = groups.find(groupName);
    if (it == groups.end()) return;
    auto& group = it->second;
    if (group.active || group.entering) return;

    group.entering = true;
    group.exiting = false;
    group.active = false;
    group.transitionStartTime = std::chrono::steady_clock::now();

    glm::vec2 offset;
    if (groupName == INVOCATIONS_OVERLAY) {
        offset = glm::vec2(-1020.f, 0.f);
    }
    else if (groupName == SCORE_BLUE_OVERLAY ||
        groupName == SCORE_RED_OVERLAY ||
        groupName == SCORE_GREEN_OVERLAY) {
        offset = glm::vec2(0.f, +300.f);
    }
    else {
        offset = glm::vec2(+1020.f, 0.f);
    }

    for (auto* e : group.entities) {
        auto t = e->get<Transform>();
        t->lPos = t->position;           // start where it is now (off-screen)
        t->tPos = t->position + offset;  
    }
}

void OverlayManager::hideOverlayGroup(const std::string& groupName) {
    auto it = groups.find(groupName);
    if (it == groups.end()) return;
    auto& group = it->second;
    if (!group.active || group.exiting) return;

    group.exiting = true;
    group.entering = false;
    group.active = false;
    group.transitionStartTime = std::chrono::steady_clock::now();

    glm::vec2 offset;
    if (groupName == INVOCATIONS_OVERLAY) {
        offset = glm::vec2(-1020.f, 0.f);
    }
    else if (groupName == SCORE_BLUE_OVERLAY ||
        groupName == SCORE_RED_OVERLAY ||
        groupName == SCORE_GREEN_OVERLAY) {
        offset = glm::vec2(0.f, +300.f);
    }
    else {
        offset = glm::vec2(+1020.f, 0.f);
    }

    for (auto* e : group.entities) {
        auto t = e->get<Transform>();
        t->lPos = t->position;            // start on-screen
        t->tPos = t->position - offset;   // move off-screen again
    }
}

void OverlayManager::toggleOverlayGroup(const std::string& groupName) {
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

void OverlayManager::applyTransition(const std::string& name, float t) {
    auto& group = groups[name];
    for (ECS::Entity* e : group.entities) {
        auto tf = e->get<Transform>();
        tf->position.x = tf->lPos.x + t * (tf->tPos.x - tf->lPos.x);
        tf->position.y = tf->lPos.y + t * (tf->tPos.y - tf->lPos.y);
    }
}

void OverlayManager::lockSelection(const std::string& groupName, bool status) {
    if (groups.find(groupName) == groups.end()) return;
    groups[groupName].selectionLocked = status;
}

bool OverlayManager::isSelectionLocked(const std::string& groupName) const {
    auto it = groups.find(groupName);
    return (it == groups.end()) ? false : it->second.selectionLocked;
}

// SCORE


void OverlayManager::addScoreEntity(int playerId, ECS::Entity* entity) {
    scoreEntities[playerId].emplace_back(entity, false); 
}

void OverlayManager::removeScoreEntity(int playerId, ECS::Entity* entity) {
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

void OverlayManager::updateScoreVisibility(int playerId, int score) {
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

EntityManager* OverlayManager::getEntityManager() {
    return entityManager;
}

void OverlayManager::clearAllOverlays() {
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

void OverlayManager::removeEntityFromAllGroups(ECS::Entity* ent) {
    for (auto& kv : groups) {
        const std::string& groupName = kv.first;
        removeOverlayEntity(groupName, ent);
    }
}