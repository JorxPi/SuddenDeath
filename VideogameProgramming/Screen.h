#pragma once

#include <string>
#include <vector>
//#include "Script.h"
#include "EntityManager.h"
#include "OverlaySystem.h"

//namespace ECS { class Entity; }

class EntityManager;

class ScreenSystem;

class OverlaySystem;

class Screen //: public Script
{
    //using Script::Script;

public:
    Screen(EntityManager* entityManager, OverlaySystem* overlaySystem)
        : entityManager(entityManager), overlaySystem(overlaySystem) {
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
    OverlaySystem* overlaySystem;


    std::vector<ECS::Entity*> managedEntities;

    void destroyManagedEntities();

    std::vector<std::pair<std::string, ECS::Entity*>> managedOverlayEntities;

    void destroyManagedOverlayEntities();

private:
    // Nombre de la pantalla a la que se quiere cambiar (vacío si no)
    std::string swapToScreenName;
};