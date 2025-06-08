#include "StartGameScreen.h"
#include "CellConstants.h"
#include "InvocationConstants.h"
#include "PassiveAbilities.h"
#include "DummyPassive.h"
#include "InvocationWithPassiveData.h"
#include <vector>
#include <algorithm>
#include <random>
#include <glm/glm.hpp>
#include <iostream>

StartGameScreen::StartGameScreen(EntityManager* entityManager, OverlayManager* overlayManager)
	: Screen(entityManager, overlayManager) {
}

void StartGameScreen::enter() {
	auto back = entityManager->createEntityWithoutScript(
		glm::vec2(600.f, 400.f), 0.f, 1.f,
		"Textures/GameBack.png", glm::vec3(1.f)
	);

	auto c1 = entityManager->createCell(CellPos::c1, "Textures/ovalo_normal.png", 74.f, 60.f, false);
	auto c2 = entityManager->createCell(CellPos::c2, "Textures/ovalo_normal.png", 74.f, 60.f, false);
	auto c3 = entityManager->createCell(CellPos::c3, "Textures/ovalo_normal.png", 74.f, 60.f, false);
	auto c4 = entityManager->createCell(CellPos::c4, "Textures/ovalo_normal.png", 74.f, 60.f, false);
	auto c5 = entityManager->createCell(CellPos::c5, "Textures/ovalo_normal.png", 74.f, 60.f, false);
	auto c6 = entityManager->createCell(CellPos::c6, "Textures/ovalo_normal.png", 74.f, 60.f, false);
	auto c7 = entityManager->createCell(CellPos::c7, "Textures/ovalo_normal.png", 74.f, 60.f, false);
	auto ha = entityManager->createCell(CellPos::ha, "Textures/hexagono_azul.png", 115.f, 100.f, false);
	auto ta = entityManager->createCell(CellPos::ta, "Textures/triangulo_blanco.png", 73.f, 64.f, true);
	auto hr = entityManager->createCell(CellPos::hr, "Textures/hexagono_rojo.png", 115.f, 100.f, false);
	auto tr = entityManager->createCell(CellPos::tr, "Textures/triangulo_blanco.png", 73.f, 64.f, true);
	auto hg = entityManager->createCell(CellPos::hg, "Textures/hexagono_verde.png", 115.f, 100.f, false);
	auto tg = entityManager->createCell(CellPos::tg, "Textures/triangulo_blanco.png", 73.f, 64.f, true);

	// === Skip Turn Button ===
	auto buttonSkip = entityManager->createEntity(
		ButtonPos::generalButtonOut, 0.f, 1.f,
		"Textures/SkipTurnButton.png", glm::vec3(1.f),
		false, glm::vec2(200.f, 85.f),
		"default", "ButtonScript"
	);
	ECS::Entity* buttonSkipEntity = buttonSkip.first;
	buttonSkipEntity->assign<BoxCollider>(200.f, 85.f, 1);

	auto skipScriptGeneric = buttonSkip.second;
	ButtonScript* buttonSkipScript = dynamic_cast<ButtonScript*>(skipScriptGeneric);
	if (buttonSkipScript) {
		buttonSkipScript->setOnClickAction([&]() {
			entityManager->getButtonManagerRef()->setSkipTurnClicked(true);
			entityManager->getButtonManagerRef()->moveSkipButtonOut();
			});
	}
	entityManager->getButtonManagerRef()->setSkipButtonRef(buttonSkipScript);

	// === Undo Move Button ===
	auto buttonUndo = entityManager->createEntity(
		ButtonPos::generalButtonOut, 0.f, 1.f,
		"Textures/UndoMoveButton.png", glm::vec3(1.f),
		false, glm::vec2(200.f, 85.f),
		"default", "ButtonScript"
	);
	ECS::Entity* buttonUndoEntity = buttonUndo.first;
	buttonUndoEntity->assign<BoxCollider>(200.f, 85.f, 1);

	auto undoGenericScript = buttonUndo.second;
	ButtonScript* buttonUndoScript = dynamic_cast<ButtonScript*>(undoGenericScript);
	if (buttonUndoScript) {
		buttonUndoScript->setOnClickAction([&]() {
			entityManager->getButtonManagerRef()->handleUndoButtonClick();
			});
	}
	entityManager->getButtonManagerRef()->setUndoButtonRef(buttonUndoScript);

	// === Game Over Button === 
	// NEED TO CHANGE THE TEXTURE

	auto buttonGameOver = entityManager->createEntity(
		ButtonPos::generalButtonOut, 0.f, 1.f,
		"Textures/SkipTurnButton.png", glm::vec3(1.f),
		false, glm::vec2(200.f, 85.f),
		"default", "ButtonScript"
	);
	ECS::Entity* buttonGameOverEntity = buttonGameOver.first;
	buttonGameOverEntity->assign<BoxCollider>(200.f, 85.f, 1);

	auto gameOverScriptGeneric = buttonGameOver.second;
	ButtonScript* buttonGameOverScript = dynamic_cast<ButtonScript*>(gameOverScriptGeneric);
	if (buttonGameOverScript) {
		buttonGameOverScript->setOnClickAction([&]() {
			requestScreenSwap("Menu");

			});
	}

	entityManager->getButtonManagerRef()->setGameOverButtonRef(buttonGameOverScript);

	//
	//setupOverlayScreen();
	//setupScoreScreen();
	//

	createToggleButtons();

	setupInvocationsOverlay();
	setupScoreOverlays();

	for (int playerId = 1; playerId <= 3; ++playerId) {
		entityManager->getGameManagerRef()->updateScoreVisibility(playerId);
	}
	//setupLogOverlay();

}

void StartGameScreen::exit() {
	// Antes de destruir la pantalla, limpiamos la listas del GameManager
	if (auto gm = entityManager->getGameManagerRef()) {
		gm->clearInvocationList();
		gm->clearCellList();
	}

	Screen::exit();
}


std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> StartGameScreen::setupCardBacks() {
	std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> cardBacks;

	glm::vec2 positions[] = {
		glm::vec2(1290.f, 300.f), glm::vec2(1420.f, 300.f), glm::vec2(1550.f, 300.f),
		glm::vec2(1680.f, 300.f), glm::vec2(1810.f, 300.f), glm::vec2(1940.f, 300.f),
		glm::vec2(1290.f, 500.f), glm::vec2(1420.f, 500.f), glm::vec2(1550.f, 500.f),
		glm::vec2(1680.f, 500.f), glm::vec2(1810.f, 500.f), glm::vec2(1940.f, 500.f)
	};

	int numPositions = sizeof(positions) / sizeof(positions[0]);

	for (int i = 0; i < numPositions; i++) {
		auto result = entityManager->createEntity(
			positions[i], 0.f, 1.f,
			"Textures/neutral_back1.png", glm::vec3(1.f),
			false, glm::vec2(107.f, 143.f),
			"default", "InvocationSelectionScript"
		);
		ECS::Entity* cardBack = result.first;

		InvocationSelectionScript* script = dynamic_cast<InvocationSelectionScript*>(result.second);
		cardBack->assign<BoxCollider>(107.f, 143.f);
		cardBack->get<Transform>()->layer = 10;
		registerOverlayEntity(INVOCATIONS_OVERLAY, cardBack);


		ECS::Entity* overlay = entityManager->createEntityWithoutScript(
			positions[i], 0.f, 1.f,
			"Textures/InvNotAv.png", glm::vec3(1.f),
			false, glm::vec2(107.f, 143.f)
		);
		overlay->get<Sprite>()->visible = false;
		overlay->get<Transform>()->layer = 11;

		registerOverlayEntity(INVOCATIONS_OVERLAY, overlay);

		script->setUnavailableOverlay(overlay);

		cardBacks.push_back(std::make_pair(cardBack, script));
	}

	return cardBacks;
}

std::vector<ECS::Entity*> StartGameScreen::setupInvocations() {
	std::vector<ECS::Entity*> invocations;

	for (int i = 0; i < Invocations::numInvocations; i++) {
		ECS::Entity* invocation = entityManager->createEntityWithoutScript(
			glm::vec2(0.f, 0.f), 0.f, 1.f,
			Invocations::data[i].texturePath, glm::vec3(1.f),
			false, Invocations::data[i].size
		);
		invocation->get<Sprite>()->visible = false;
		invocation->get<Transform>()->layer = 10;

		registerOverlayEntity(INVOCATIONS_OVERLAY, invocation);

		invocations.push_back(invocation);
	}

	return invocations;
}

void StartGameScreen::randomizeInvocations(std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>>& cardBacks, std::vector<ECS::Entity*>& invocations) {
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(invocations.begin(), invocations.end(), g);

	std::vector<std::unique_ptr<PassiveAbility>> passives;

	// La primera es passiveNine
	passives.emplace_back(std::make_unique<PassiveNine>());

	//segona passiveOne
	passives.emplace_back(std::make_unique<PassiveOne>());

	//tercera passiveFive
	passives.emplace_back(std::make_unique<PassiveFive>());

	//cuarta passiveTen
	passives.emplace_back(std::make_unique<PassiveTen>());

	//cinquena passiveEleven
	passives.emplace_back(std::make_unique<PassiveEleven>());

	//sisena passiveFour
	passives.emplace_back(std::make_unique<PassiveFour>());

	//setena passiveSix
	passives.emplace_back(std::make_unique<PassiveSix>());

	//vuitena passiveSeven
	passives.emplace_back(std::make_unique<PassiveSeven>());

	//novena passiveTwo
	passives.emplace_back(std::make_unique<PassiveTwo>());

	//desena passiveFifteen
	passives.emplace_back(std::make_unique<PassiveFifteen>());

	// La resta DummyPassive
	for (size_t i = 10; i < invocations.size(); i++) {
		passives.emplace_back(std::make_unique<DummyPassive>());
	}

	std::shuffle(passives.begin() + 10, passives.end(), g);

	invocationPool.clear();
	for (size_t i = 0; i < invocations.size() && i < passives.size(); i++) {
		InvocationWithPassiveData data;
		data.filepath = invocations[i]->get<Sprite>()->filepath; 
		data.passive = std::move(passives[i]);                  
		invocationPool.push_back(std::move(data));
	}


	for (int i = 0; i < cardBacks.size() && i < invocations.size(); i++) {
		InvocationSelectionScript* script = cardBacks[i].second;
		ECS::Entity* invocation = invocations[i];

		invocation->get<Transform>()->position = cardBacks[i].first->get<Transform>()->position;

		script->asociateInvoc(invocations[i]);
		script->setInvocationData(invocationPool[i].filepath, invocationPool[i].passive.get());

		entityManager->settingCardsSelectionList(script);
	}
}

void StartGameScreen::createToggleButtons() {
	{
		auto result = entityManager->createEntity(
			glm::vec2(1200.f, 400.f), 0.f, 1.f,
			"Textures/OverlayInvocButton.png", glm::vec3(1.f),
			true, glm::vec2(64.f, 64.f),
			"default", "ButtonScript"
		);
		buttonToggleInvocations = result.first;
		buttonToggleInvocations->assign<BoxCollider>(64.f, 64.f, 1);

		registerOverlayEntity(INVOCATIONS_OVERLAY, buttonToggleInvocations);

		ButtonScript* bs = static_cast<ButtonScript*>(result.second);
		if (bs) {
			bs->setOnClickAction([this]() {
				auto gm = entityManager->getGameManagerRef();
				if (gm && gm->getIsSelectingInvocation()) {
					return;
				}
				overlayManager->toggleOverlayGroup(INVOCATIONS_OVERLAY);
				overlayManager->lockSelection(INVOCATIONS_OVERLAY, true);

				});
		}
	}

	// Three smaller buttons at top-right for each player's score
	glm::vec2 scoreButtonPositions[3] = {
		{930.f, 0.f}, // Blue
		{1020.f, 0.f}, // Red
		{1110.f, 0.f}, // Green
	};

	for (int i = 0; i < 3; ++i) {
		auto result = entityManager->createEntity(
			scoreButtonPositions[i], 0.f, 1.f,
			(i == 0 ? "Textures/OverlayScoreBlueButton.png" :
				i == 1 ? "Textures/OverlayScoreRedButton.png" :
				"Textures/OverlayScoreGreenButton.png"),
			glm::vec3(1.f),
			false, glm::vec2(48.f, 48.f),
			"default", "ButtonScript"
		);
		buttonToggleScores[i] = result.first;
		buttonToggleScores[i]->assign<BoxCollider>(48.f, 48.f, 1);

		buttonToggleScores[i]->get<Transform>()->layer = 20;

		const std::string groupName =
			(i == 0 ? SCORE_BLUE_OVERLAY
				: i == 1 ? SCORE_RED_OVERLAY
				: SCORE_GREEN_OVERLAY);

		registerOverlayEntity(groupName, buttonToggleScores[i]);

		ButtonScript* bs = static_cast<ButtonScript*>(result.second);
		if (bs) {
			bs->setOnClickAction([this, groupName]() {
				// Use a different group name for each color
				overlayManager->toggleOverlayGroup(groupName);
				});
		}
	}
	/*
	// One button at left-center for the Game Log
	{
		auto result = entityManager->createEntity(
			glm::vec2(50.f, 300.f), 0.f, 1.f,
			"Textures/ButtonToggleLog.png", glm::vec3(1.f),
			true, glm::vec2(48.f, 48.f),
			"default", "ButtonScript"
		);
		buttonToggleLog = result.first;
		registerEntity(buttonToggleLog);

		buttonToggleLog->assign<BoxCollider>(48.f, 48.f, 1);
		ButtonScript* bs = static_cast<ButtonScript*>(result.second);
		if (bs) {
			bs->setOnClickAction([this]() {
				overlayManager->toggleOverlayGroup(LOG_OVERLAY);
				});
		}
	}*/
}

void StartGameScreen::setupInvocationsOverlay() {
	if (auto gm = entityManager->getGameManagerRef()) {
		gm->clearInvocationSelectionList();
	}

	// 1) Blanket semitransparente
	ECS::Entity* blanket = entityManager->createEntityWithoutScript(
		glm::vec2(1620.f, 400.f), 0.f, 1.f,
		"Textures/OverlayBlanket.png", glm::vec3(1.f), false, glm::vec2(835.f, 432.f)
	);
	blanket->get<Transform>()->layer = 10;
	registerOverlayEntity(INVOCATIONS_OVERLAY, blanket);

	// 2) Preparar cardBacks e invocations
	auto cardBacks = setupCardBacks();
	auto invocations = setupInvocations();
	randomizeInvocations(cardBacks, invocations);

	// 3) Registrar cada card back y su overlay InvNotAv en INVOCATIONS_OVERLAY
	for (auto& pair : cardBacks) {
		ECS::Entity* cardBackEntity = pair.first;
		registerOverlayEntity(INVOCATIONS_OVERLAY, cardBackEntity);

		InvocationSelectionScript* script = pair.second;
		ECS::Entity* overlay = script->getUnavailableOverlay();
		if (overlay) {
			registerOverlayEntity(INVOCATIONS_OVERLAY, overlay);
		}
		script->setOverlayManager(overlayManager);
	}

	// 4) Registrar cada invocation en INVOCATIONS_OVERLAY
	for (ECS::Entity* inv : invocations) {
		registerOverlayEntity(INVOCATIONS_OVERLAY, inv);
	}
}

void StartGameScreen::setupScoreOverlays() {

	glm::vec2 columnStart[3] = {
		{930.f, -150.f}, 
		{1020.f, -150.f}, 
		{1110.f, -150.f} 
	};

	static const char* bgTextures[3] = {
		"Textures/BlueScoreOverlay.png",
		"Textures/RedScoreOverlay.png",
		"Textures/GreenScoreOverlay.png"
	};

	for (int i = 0; i < 3; ++i) {
		const std::string groupName =
			(i == 0 ? SCORE_BLUE_OVERLAY
				: i == 1 ? SCORE_RED_OVERLAY
				: SCORE_GREEN_OVERLAY);

		ECS::Entity* scoreBg = entityManager->createEntityWithoutScript(
			columnStart[i], 
			0.f, 1.f,
			bgTextures[i],                       
			glm::vec3(1.f), false, glm::vec2(58.f, 300.f)
		);
		scoreBg->get<Transform>()->layer = 20;

		registerOverlayEntity(groupName, scoreBg);

		const int numRows = 20, numCols = 3;
		const float vSpacing = 14.2f, hSpacing = 15.8f;

		for (int row = 0; row < numRows; ++row) {
			for (int col = 0; col < numCols; ++col) {
				glm::vec2 ptPos =
					columnStart[i] + glm::vec2(-16.f, -130.f)
					+ glm::vec2(col * hSpacing, row * vSpacing);

					ECS::Entity* pt = entityManager->createEntityWithoutScript(
						ptPos,
						0.f, 1.f,
						"Textures/SuddenPoint.png",
						glm::vec3(1.f), false, glm::vec2(12.5f, 12.5f)
					);
					pt->get<Sprite>()->visible = false;
					pt->get<Transform>()->layer = 21;

					registerOverlayEntity(groupName, pt);
					overlayManager->addScoreEntity(i + 1, pt);
			}
		}
	}
}

/*void StartGameScreen::setupLogOverlay() {
	// Create a panel (or several text lines), all off?screen initially.
	// We assume the GameLogManager draws text onto some container entity. 
	// Here, we might create a large semi?transparent rectangle off?screen, etc.
	ECS::Entity* logBg = entityManager->createEntityWithoutScript(
		glm::vec2(100.f, 100.f) + glm::vec2(-1500.f, 0.f), // off?screen left
		0.f, 1.f,
		"Textures/LogPanel.png", glm::vec3(1.f)
	);
	logBg->get<Transform>()->layer = 15;
	registerOverlayEntity(logBg);
	overlayManager->addOverlayEntity(LOG_OVERLAY, logBg);

	for (int i = 0; i < 6; ++i) {
		ECS::Entity* logLine = entityManager->createEntityWithoutScript(
			glm::vec2(120.f, 120.f + i * 20.f) + glm::vec2(-1500.f, 0.f),
			0.f, 1.f,
			"Textures/TransparentTextQuad.png", glm::vec3(1.f)
		);
		logLine->get<Transform>()->layer = 16;
		registerOverlayEntity(logLine);
		overlayManager->addOverlayEntity(LOG_OVERLAY, logLine);
		// You would attach a small component to render each line of text via TextRenderer later.
	}
}*/
