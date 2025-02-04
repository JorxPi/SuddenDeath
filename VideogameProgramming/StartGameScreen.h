#pragma once

#include "Screen.h"
#include "OverlayManager.h"
#include "InvocationWithPassiveData.h"
#include <vector>
#include <string>
#include <memory>

class StartGameScreen : public Screen {
public:
	StartGameScreen(EntityManager* entityManager, OverlayManager* overlayManager);

	void enter() override;
	void exit() override;
	void setupOverlayScreen();
	void setupScoreScreen();
	std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> setupCardBacks();
	std::vector<ECS::Entity*> setupInvocations();
	void randomizeInvocations(std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>>& cardBacks, std::vector<ECS::Entity*>& invocations);

private:
	OverlayManager* overlayManager;

	std::vector<ECS::Entity*> entities;
	std::vector<InvocationWithPassiveData> invocationPool;
};