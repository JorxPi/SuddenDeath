#pragma once

#include "Screen.h"

class MenuScreen : public Screen 
{
public:
	MenuScreen(EntityManager* entityManager);

	void enter() override;
	void exit() override;

private:
	std::vector<ECS::Entity*> entities;
};
