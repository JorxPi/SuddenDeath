#include "EntityManager.h"
#include "InvocationConstants.h"
#include <functional>
#include <vector>

EntityManager::EntityManager(GLFWwindow* window, ECS::World* world, ScriptSystem* scriptSystemx)
    : window(window), world(world), scriptSystem(scriptSystemx) {

}

std::pair<ECS::Entity*, Script*> EntityManager::createEntity(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, bool autoSize, glm::vec2 size, const char* shaderName, const std::string& scriptName) {
    ECS::Entity* entity = world->create();
    entity->assign<Transform>(position, rotation, scale);
    entity->assign<Sprite>(filepath, color, autoSize, size, shaderName);

	Script* script = nullptr;
	if (!scriptName.empty()) {
		script = ScriptAssigner::createScript(scriptName, entity, window, world);
		entity->assign<ScriptComponent>(scriptManagerRef->AddScript(script));
	}

	return { entity, script };
}

ECS::Entity* EntityManager::createEntityWithoutScript(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, bool autoSize, glm::vec2 size, const char* shaderName) {
	ECS::Entity* entity = world->create();
	entity->assign<Transform>(position, rotation, scale);
	entity->assign<Sprite>(filepath, color, autoSize, size, shaderName);

	return entity;
}

void EntityManager::createInvocation(glm::vec2 position, int coliderId, int attack, const char* text_back, const char* text_inv, const char* text_att, int playerOwner) {
	glm::vec2 defaultSize(61.f, 92.f);
	glm::vec2 invocSize = defaultSize;

	for (int i = 0; i < Invocations::numInvocations; ++i) {
		if (strcmp(Invocations::data[i].texturePath, text_inv) == 0) {
			invocSize = Invocations::data[i].size;
			break;
		}
	}


	ECS::Entity* spawn_card = createEntityWithoutScript(glm::vec2(0.f, 15.f), 0.f, 1.f, text_back, glm::vec3(1., 1., 1.), false, glm::vec2(119., 159.));

	auto result_gobobo = createEntity(position, 0.f, 1.f, text_inv, glm::vec3(1., 1., 1.), false, invocSize, "default", "InvocationScript");
	ECS::Entity* spawn_gobobo = result_gobobo.first;
	Script* generic_script = result_gobobo.second;

	InvocationScript* invoc_script = dynamic_cast<InvocationScript*>(generic_script);

	invoc_script->setPlayerOwner(playerOwner);

	spawn_gobobo->assign<BoxCollider>(invocSize.x, invocSize.y, coliderId);

	//InvocationScript* invoc_script = new InvocationScript(window, world, spawn_gobobo);
	//spawn_gobobo->assign<ScriptComponent>(scriptManager->AddScript(invoc_script));
	invoc_script->startScript();
	gameManagerRef->addInvocationScriptToList(invoc_script);
	invoc_script->setInvocAttack(attack);
	invoc_script->setButtonManagerRef(buttonManagerRef);
	invoc_script->setGameManagerRef(gameManagerRef);

	ECS::Entity* att = createEntityWithoutScript(glm::vec2(0.f, 70.f), 0.f, 1.f, text_att, glm::vec3(1., 1., 1.), false, glm::vec2(14., 36.));

	invoc_script->setInvocRefs(spawn_gobobo, spawn_card, att);

	spawn_card->get<Transform>()->assignParent(&spawn_gobobo->get<Transform>().get());
	att->get<Transform>()->assignParent(&spawn_gobobo->get<Transform>().get());
}

void EntityManager::createInvocationWithPassive(glm::vec2 position, int coliderId, int attack, const char* text_back, const char* text_inv, const char* text_att, int playerOwner, PassiveAbility* passive) {
	// Use the existing createInvocation logic
	createInvocation(position, coliderId, attack, text_back, text_inv, text_att, playerOwner);

	// Get the last created invocation script (assuming the last script added to the list is the current one)
	if (!gameManagerRef || gameManagerRef->getInvocationScripts().empty()) {
		std::cerr << "Error: Unable to assign passive because invocation list is empty!" << std::endl;
		return;
	}

	InvocationScript* latestInvocationScript = gameManagerRef->getInvocationScripts().back();
	if (latestInvocationScript) {
		latestInvocationScript->setPassiveAbility(passive); // Assign the passive
	}
	else {
		std::cerr << "Error: Unable to assign passive to the latest invocation script!" << std::endl;
	}
}

ECS::Entity* EntityManager::createCell(glm::vec2 position, const char* text_cell_type, float col_w, float col_h) {
	auto result_cell = createEntity(position, 0.f, 1.f, text_cell_type, glm::vec3(1., 1., 1.), true, glm::vec2(1.0F, 1.0F), "default", "CellScript");
	ECS::Entity* cell = result_cell.first;
	Script* generic_script = result_cell.second;

	CellScript* cell_script = dynamic_cast<CellScript*>(generic_script);

	cell->assign<BoxCollider>(col_w, col_h, 1);

	//CellScript* cell_script = new CellScript(window, world, cell);
	//cell->assign<ScriptComponent>(scriptManagerRef->AddScript(cell_script));
	gameManagerRef->addCellToList(cell_script);

	cell_script->setCellRef(cell);

	return cell;
}

void EntityManager::destroyEntity(Entity* entity) {
	world->destroy(entity);
}

void EntityManager::setManagerRefs(GameManager* gmr, ButtonManager* bmr, ScriptManager* smr) {
	gameManagerRef = gmr;
	buttonManagerRef = bmr;
	scriptManagerRef = smr;

}

ButtonManager* EntityManager::getButtonManagerRef() {
	return buttonManagerRef;
}

void EntityManager::settingCardsSelectionList(InvocationSelectionScript* invssc) {
	gameManagerRef->addInvocationSelectionScriptToList(invssc);
}