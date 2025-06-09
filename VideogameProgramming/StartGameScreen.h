#pragma once

#include "Screen.h"
#include "InvocationWithPassiveData.h"
#include <vector>
#include <string>
#include <memory>

static constexpr char INVOCATIONS_OVERLAY[] = "InvocationsOverlay";
static constexpr char SCORE_BLUE_OVERLAY[] = "ScoreBlueOverlay"; 
static constexpr char SCORE_RED_OVERLAY[] = "ScoreRedOverlay"; 
static constexpr char SCORE_GREEN_OVERLAY[] = "ScoreGreenOverlay";
static constexpr char INFO_OVERLAY[] = "InfoOverlay";
//static constexpr char LOG_OVERLAY[] = "LogOverlay"; [NO-INVOCATIONS]

class StartGameScreen : public Screen {
public:
	StartGameScreen(EntityManager* entityManager, OverlayManager* overlayManager);

	void enter() override;
	void exit() override;

	std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> setupCardBacks();
	std::vector<ECS::Entity*> setupInvocations();
	void randomizeInvocations(std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>>& cardBacks, std::vector<ECS::Entity*>& invocations);

	void createToggleButtons();	
	void setupInvocationsOverlay();
	void setupScoreOverlays(); 
	void setupInfoOverlay();
	//void setupLogOverlay(); [NO-INVOCATIONS]

private:
	std::vector<InvocationWithPassiveData> invocationPool;

	ECS::Entity* buttonToggleInvocations;
	ECS::Entity* buttonToggleScores[3];
	ECS::Entity* buttonToggleInfo;
	//ECS::Entity* buttonToggleLog; [NO-INVOCATIONS]
};