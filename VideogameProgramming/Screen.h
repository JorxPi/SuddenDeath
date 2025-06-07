#pragma once

#include <string>
#include <vector>
//#include "Script.h"
#include "EntityManager.h"
#include "OverlayManager.h"

//namespace ECS { class Entity; }

class EntityManager;

class ScreenManager;

class OverlayManager;

class Screen //: public Script
{
    //using Script::Script;

public:
    Screen(EntityManager* entityManager, OverlayManager* overlayManager)
        : entityManager(entityManager), overlayManager(overlayManager) {
    }

    virtual ~Screen() = default;

    virtual void enter() = 0;
    virtual void exit();

    void requestScreenSwap(const std::string& screenName) { swapToScreenName = screenName; }
    const std::string& getRequestedSwap() const { return swapToScreenName; }

    void registerEntity(ECS::Entity* ent);
    void unregisterEntity(ECS::Entity* ent);

    void registerOverlayEntity(const std::string& groupName, ECS::Entity* ent);
    void unregisterOverlayEntity(const std::string& groupName, ECS::Entity* ent);

    void unregisterOverlayEntityFromAllGroups(ECS::Entity* ent);

protected:
    EntityManager* entityManager;
    OverlayManager* overlayManager;


    std::vector<ECS::Entity*> managedEntities;

    void destroyManagedEntities();

    std::vector<std::pair<std::string, ECS::Entity*>> managedOverlayEntities;

    void destroyManagedOverlayEntities();

private:
    // Nombre de la pantalla a la que se quiere cambiar (vacío si no)
    std::string swapToScreenName;
};