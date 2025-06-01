#pragma once

#include "Screen.h"

class MenuScreen : public Screen 
{
public:
	MenuScreen(EntityManager* entityManager, OverlayManager* overlayManager);

	void enter() override;

private:

};
