#pragma once

#include "Script.h"
#include "OverlayManager.h"
#include "PassiveAbilities.h"

class OverlayManager;

class InvocationSelectionScript : public Script
{
    using Script::Script;

public:
    void tickScript(float deltaTime) override;
    void setInvocAsociatedVisible();
    void invocationSelected();
    void asociateInvoc(Entity* enty);
    bool getCardSelected() const;
    const char* getSpriteFilepath();
    PassiveAbility* getPassiveAbility();
    void setInvocationData(const std::string& filepath, PassiveAbility* passive);

    void setCardSelected(bool status);

    void setUnavailable(bool unavailable);  // New: Block selection visually and logically
    void setUnavailableOverlay(ECS::Entity* overlay);
    ECS::Entity* getUnavailableOverlay();

    void setOverlayManager(OverlayManager* manager);

    void updateHoverText();

private:
    OverlayManager* overlayManager = nullptr;
    GameManager* gameManagerRef = nullptr;

    Entity* invocAsociated;
    Entity* unavailableOverlay;  // New: Reference to "InvNotAv" overlay entity

    std::string filepath;               
    PassiveAbility* passive = nullptr;

    bool discovered = false;
    bool card_selected = false;
    bool isUnavailable = false; 
    bool isHovered = false;

};
