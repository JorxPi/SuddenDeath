#pragma once

#include "Script.h"
#include <functional>

class ButtonScript : public Script
{
    using Script::Script;

public:
    //GENERAL LOGIC
    void tickScript(float deltaTime) override;
    void setOnClickAction(std::function<void()> action);

    //GETTERS
    Entity* getEntity() const;

private:
    std::function<void()> onClickAction;
    bool isHovered = false;


};