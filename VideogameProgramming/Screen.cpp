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

void Screen::registerOverlayEntity(ECS::Entity* ent) {
    overlayManager->addOverlayEntity(ent);
    auto it = std::find(managedOverlayEntities.begin(), managedOverlayEntities.end(), ent);
    if (it == managedOverlayEntities.end()) {
        managedOverlayEntities.push_back(ent);
    }
}

void Screen::unregisterOverlayEntity(ECS::Entity* ent) {
    overlayManager->removeOverlayEntity(ent);
    auto it = std::remove(managedOverlayEntities.begin(), managedOverlayEntities.end(), ent);
    if (it != managedOverlayEntities.end()) {
        managedOverlayEntities.erase(it, managedOverlayEntities.end());
    }
}

void Screen::destroyManagedEntities() {
    for (auto e : managedEntities) {
        entityManager->destroyEntity(e);
    }
    managedEntities.clear();
}

void Screen::destroyManagedOverlayEntities() {
    for (auto e : managedOverlayEntities) {
        overlayManager->removeOverlayEntity(e);
        entityManager->destroyEntity(e);
    }
    managedOverlayEntities.clear();
}

void Screen::exit() {
    destroyManagedEntities();
    destroyManagedOverlayEntities();

    swapToScreenName.clear();
}