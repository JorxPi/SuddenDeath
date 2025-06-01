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

	//entityManager->createInvocation(glm::vec2(670.f, 110.f), 1, 1, "Textures/blue_back.png", "Textures/inv_Gobobo.png", "Textures/att1.png", 1);

	//entityManager->createInvocation(glm::vec2(950.f, 600.f), 2, 1, "Textures/red_back.png", "Textures/inv_Gobobo.png", "Textures/att1.png", 2);

	//entityManager->createInvocation(glm::vec2(300.f, 570.f), 3, 1, "Textures/green_back.png", "Textures/inv_Gobobo.png", "Textures/att1.png", 3);


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

	setupOverlayScreen();
	setupScoreScreen();

		
}


std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> StartGameScreen::setupCardBacks() {
	std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> cardBacks;

	glm::vec2 positions[] = {
		glm::vec2(1800.f, 275.f), glm::vec2(1950.f, 275.f), glm::vec2(2100.f, 275.f),
		glm::vec2(2250.f, 275.f), glm::vec2(2400.f, 275.f), glm::vec2(2550.f, 275.f),
		glm::vec2(1800.f, 525.f), glm::vec2(1950.f, 525.f), glm::vec2(2100.f, 525.f),
		glm::vec2(2250.f, 525.f), glm::vec2(2400.f, 525.f), glm::vec2(2550.f, 525.f)
	};

	int numPositions = sizeof(positions) / sizeof(positions[0]);

	for (int i = 0; i < numPositions; i++) {
		auto result = entityManager->createEntity(
			positions[i], 0.f, 1.f,
			"Textures/neutral_back1.png", glm::vec3(1.f),
			false, glm::vec2(119.f, 159.f),
			"default", "InvocationSelectionScript"
		);
		ECS::Entity* cardBack = result.first;

		InvocationSelectionScript* script = dynamic_cast<InvocationSelectionScript*>(result.second);
		cardBack->assign<BoxCollider>(119.f, 159.f);
		cardBack->get<Transform>()->layer = 10;
		registerOverlayEntity(cardBack);


		ECS::Entity* overlay = entityManager->createEntityWithoutScript(
			positions[i], 0.f, 1.f,
			"Textures/InvNotAv.png", glm::vec3(1.f),
			false, glm::vec2(119.f, 159.f)
		);
		overlay->get<Sprite>()->visible = false;
		overlay->get<Transform>()->layer = 11;  

		registerOverlayEntity(overlay);

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

		registerOverlayEntity(invocation);

		invocations.push_back(invocation);
	}

	return invocations;
}

void StartGameScreen::randomizeInvocations(std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>>& cardBacks, std::vector<ECS::Entity*>& invocations) {
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(invocations.begin(), invocations.end(), g);

	// Fill with DummyPassive for now
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
		data.filepath = invocations[i]->get<Sprite>()->filepath; // Assign filepath
		data.passive = std::move(passives[i]);                   // Move passive ownership
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

void StartGameScreen::setupOverlayScreen() {
	ECS::Entity* blanket = entityManager->createEntityWithoutScript(
		glm::vec2(2200.f, 400.f), 0.f, 1.f,
		"Textures/OverlayBlanket.png", glm::vec3(1.f)
	);
	blanket->get<Transform>()->layer = 10;

	registerOverlayEntity(blanket);

	auto cardBacks = setupCardBacks();
	auto invocations = setupInvocations();
	randomizeInvocations(cardBacks, invocations);

	//overlayManager->addOverlayEntity(blanket);
	for (int i = 0; i < cardBacks.size(); i++) {
		InvocationSelectionScript* script = cardBacks[i].second;
		if (script) {
			ECS::Entity* overlay = script->getUnavailableOverlay();
			if (overlay) {
				registerOverlayEntity(overlay);
			}
			script->setOverlayManager(overlayManager);
		}
	}
	for (int i = 0; i < invocations.size(); i++) {
		registerOverlayEntity(invocations[i]);
	}
}

void StartGameScreen::setupScoreScreen() {
	ECS::Entity* scoreBackground = entityManager->createEntityWithoutScript(
		glm::vec2(600.f, 400.f), 0.f, 1.f,
		"Textures/ScoreTab.png", glm::vec3(1.f)
	);
	scoreBackground->get<Sprite>()->visible = false;
	scoreBackground->get<Transform>()->layer = 20;

	registerOverlayEntity(scoreBackground);
	overlayManager->addScoreEntity(-1, scoreBackground);


	glm::vec2 playerColumnStartPositions[] = {
		glm::vec2(459.f, 241.f), 
		glm::vec2(578.f, 241.f), 
		glm::vec2(697.f, 241.f)  
	};

	const int numRows = 20; 
	const int numColumns = 3; 
	const float verticalSpacing = 20.5f; 
	const float horizontalSpacing = 23.f; 

	for (int playerIndex = 0; playerIndex < 3; playerIndex++) {
		glm::vec2 startPos = playerColumnStartPositions[playerIndex];

		for (int row = 0; row < numRows; row++) {
			for (int col = 0; col < numColumns; col++) {
				glm::vec2 position = glm::vec2(
					startPos.x + (col * horizontalSpacing), // X position
					startPos.y + (row * verticalSpacing) // Adjust Y position
				);


				ECS::Entity* playerPoint = entityManager->createEntityWithoutScript(
					position, 0.f, 1.f,
					"Textures/SuddenPoint.png", glm::vec3(1.f)
				);
				playerPoint->get<Sprite>()->visible = false; // Initially invisible
				playerPoint->get<Transform>()->layer = 21;   // Above the background layer

				registerOverlayEntity(playerPoint);
				// Add it to the overlay manager under the correct player ID
				overlayManager->addScoreEntity(playerIndex + 1, playerPoint); // Player ID: 1, 2, 3
			}
		}
	}
}



/*void StartGameScreen::tickScript(float dt) {

}*/
