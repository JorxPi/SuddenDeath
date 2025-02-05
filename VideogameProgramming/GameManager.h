#pragma once

#include "Script.h"
#include "Ruleta.h"
#include "InvocationScript.h"
#include "CellScript.h"
#include "CoinScript.h"
#include "RuletaBatalla.h"
#include "RuletaAttacks.h"
#include "InvocationSelectionScript.h"
#include "ButtonManager.h"
#include "EntityManager.h"
#include "ScreenManager.h"
#include "PassiveAbilities.h"
#include "PassiveNine.h"
#include "PassiveOne.h"
#include "PassiveFive.h"
#include "PassiveTen.h"
#include "PassiveEleven.h"
#include "TextRenderer.h"
#include <vector>

using namespace std;

class Ruleta;

class InvocationScript;

class CellScript;

class CoinScript;

class RuletaBatalla;

class RuletaAttacks;

class InvocationSelectionScript;

class ButtonManager;

class EntityManager;

class ScreenManager;

class GameManager {

public:
	GameManager(GLFWwindow* window, ECS::World* world, EntityManager* entityManager);

	ECS::Entity* createEntity(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "default");

	void activate(bool state);

	void update(float deltaTime);

	void RenderGameText(const std::string& text, float x, float y, float scale, glm::vec3 color);

	//GENERAL LOGIC
	void invocInvocation(int invocAttack, const char* filepath, PassiveAbility* passive);
	void highlightSprite(int cellPos);
	void changeTurnAndReset();
	bool checkIfAllyInvocInCell(int cellPos);
	bool checkIfEnemyInvocInCell(int cellPos);
	glm::vec3 decideRoulColor(int colId);
	void checkBattle(InvocationScript* invocScript);
	void skipButtonGameLogic();
	void addPlayerPoints(int playerIndex, int points);
	void awardBattlePoints(int winnerIndex, int loserIndex);
	void awardInvocationPoints(InvocationScript* invocScript);
	void updateScoreVisibility(int playerScoring);

	void addInvocationToBoard(const std::string& invocationId);
	void removeInvocationFromBoard(const std::string& invocationId);
	bool isInvocationOnBoard(const std::string& invocationId);

	template <typename PassiveType>
	void handlePassiveEvent(InvocationScript* invocation, std::function<void(PassiveType*)> eventHandler);

	//SECOND COIN PASSIVE
	bool isFirstFlipTails();
	void spawnExtraCoin();
	bool isExtraCoinActive();
	void markExtraCoinUsed();

	//ENTITY CREATION
	void spawnCoin();
	void spawnRuletaAttacks();
	void spawnRuletaBatalla(glm::vec3 color1, glm::vec3 color2, float atac1, float atac2);
	const char* returnAttackText(int invocAttack);

	//ENTITY DESTRUCTION
	void destroyRuletaAttacks();
	void destroyRuletaBatalla();
	void eliminateInvocation(int cellPosInArray);

	//LISTS FILLING / CLEARING
	void addInvocationScriptToList(InvocationScript* invsc);
	void addCellToList(CellScript* cellsc);
	void clearHighlightedList();
	void addInvocationSelectionScriptToList(InvocationSelectionScript* invssc);

	//INVOCATION SELECTION
	void startInvocationSelection();
	void endInvocationSelection();

	//GETTERS
	int getCurrentPlayerTurn();
	std::vector<Entity*> getHighlightedEntities();
	std::vector<Entity*> getAllCellEntities();
	std::vector<CellScript*> getAllCellScripts();
	std::vector<InvocationScript*>& getInvocationScripts();
	int getInvocationIndex(InvocationScript* invocation);

	//SETTERS
	void setTextRenderer(TextRenderer* textRenderer);
	void setButtonManagerRef(ButtonManager* bmr);
	void setScreenManagerRef(ScreenManager* smr);
	void setRuletaAttacksRef(RuletaAttacks* rcr, Entity* rent, Entity* pointent);
	void setRuletaBatallaRef(RuletaBatalla* rbcr, Entity* rbent, Entity* pointent);
	void setCoinRef(CoinScript* ccr, Entity* cent);
	void setCurrentPlayerInvocationsListRef();



private:
	//SETUP
	GLFWwindow* window;
	ECS::World* world;
	EntityManager* entityManager;
	TextRenderer* textRendererRef;

	bool isActive = false;

	//ENTITIESS
	Entity* ruletaAttacksEntRef;
	Entity* pointerEntRef;
	Entity* ruletaBatallaEntRef;
	Entity* coinEntRef;

	//SCRIPTS
	ButtonManager* buttonRef;
	ScreenManager* screenManagerRef;
	RuletaAttacks* ruletaAttacksRef;
	RuletaBatalla* ruletaBatallaRef;
	CoinScript* coinScriptRef;

	//SCRIPTS LISTS
	std::vector<InvocationScript*> invocationsListRef;
	std::vector<InvocationScript*> currentPlayerInvocationsListRef;
	std::vector<CellScript*> cellListRef;
	std::vector<InvocationSelectionScript*> invocCardListRef;

	//ENTITIES LISTS
	std::vector<Entity*> highlightedEntities;
	std::vector<std::string> invocationsOnBoard;

	//CONTROL BOOLEANS
	bool movementBlocked = false;
	bool currentPlayerInvocationsListSetted = false;
	bool ruletaAttacksDestroyed = false;
	bool ruletaBatallaDestroyed = false;
	bool allInvocs = false;
	bool battleing = false;
	bool ruletaAttacksSpawned = false;
	bool ruletaBatallaSpawned = false;
	bool invocDestroyed = true;
	bool invocInvocated = false;
	bool allInvocsMoved = false;
	bool deferredCoinSpawn = false;
	bool checkingIfBattle = false;
	bool skipButtonIn = false;
	bool selectingInvocation = false;
	bool delayStarted = false;
	bool extraCoinActive = false;

	//SCORE
	std::map<int, int> playerScores; // Player ID -> Points

	//OTHER
	int currentPlayerTurn = 1;
	int ruletaBatallaWinner;
	int posListInvoc1, posListInvoc2;
	int currentInvocationMoving =  -1;
	int invocationToSpawnPos = -1;

	std::chrono::steady_clock::time_point battleTime;
	std::chrono::steady_clock::time_point selectionTime;
};
