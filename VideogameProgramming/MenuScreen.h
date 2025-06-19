#pragma once

#include "Screen.h"

class MenuScreen : public Screen 
{
public:
	MenuScreen(EntityManager* entityManager, OverlaySystem* overlaySystem);

	void enter() override;

private:

};
