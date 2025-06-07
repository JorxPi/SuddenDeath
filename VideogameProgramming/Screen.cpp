#include "Screen.h"

void Screen::registerEntity(ECS::Entity* ent) {
    auto it = std::find(managedEntities.begin(), managedEntities.end(), ent);
    if (it == managedEntities.end()) {
        managedEntities.push_back(ent);
    }
}

void Screen::unregisterEntity(ECS::Entity* ent) {
    auto it = std::remove(managedEntities.begin(), managedEntities.end(), ent);
    if (it != managedEntities.end()) {
        managedEntities.erase(it, managedEntities.end());
    }
}

void Screen::registerOverlayEntity(const std::string& groupName, ECS::Entity* ent) {
    overlayManager->addOverlayEntity(groupName, ent);
    managedOverlayEntities.emplace_back(groupName, ent);
}

void Screen::unregisterOverlayEntity(const std::string& groupName, ECS::Entity* ent) {
    overlayManager->removeOverlayEntity(groupName, ent);

    managedOverlayEntities.erase(
        std::remove_if(
            managedOverlayEntities.begin(),
            managedOverlayEntities.end(),
            [&](auto& pair) {
                return pair.first == groupName && pair.second == ent;
            }),
        managedOverlayEntities.end()
    );
}

void Screen::unregisterOverlayEntityFromAllGroups(ECS::Entity* ent) {
    for (auto& kv : managedOverlayEntities) {
        const std::string& groupName = kv.first;
        ECS::Entity* ePtr = kv.second;
        if (ePtr == ent) {
            overlayManager->removeOverlayEntity(groupName, ent);
        }
    }
    managedOverlayEntities.erase(
        std::remove_if(
            managedOverlayEntities.begin(),
            managedOverlayEntities.end(),
            [&](auto& pair) {
                return pair.second == ent;
            }),
        managedOverlayEntities.end()
    );
}

void Screen::destroyManagedEntities() {
    for (auto e : managedEntities) {
        entityManager->destroyEntity(e);
    }
    managedEntities.clear();
}

void Screen::destroyManagedOverlayEntities() {
    for (auto& pair : managedOverlayEntities) {
        const std::string& groupName = pair.first;
        ECS::Entity* ent = pair.second;

        overlayManager->removeOverlayEntity(groupName, ent);

        entityManager->destroyEntity(ent);
    }
    managedOverlayEntities.clear();
}

void Screen::exit() {
    destroyManagedEntities();

    if (overlayManager) {
        overlayManager->clearAllOverlays();
    }

    destroyManagedOverlayEntities();

    swapToScreenName.clear();
}