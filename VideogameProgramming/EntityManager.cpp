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

	if (screenSystemRef) {
		Screen* s = screenSystemRef->getCurrentScreen();
		if (s) {
			s->registerEntity(entity);
		}
	}

	return { entity, script };
}

ECS::Entity* EntityManager::createEntityWithoutScript(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, bool autoSize, glm::vec2 size, const char* shaderName) {
	ECS::Entity* entity = world->create();
	entity->assign<Transform>(position, rotation, scale);
	entity->assign<Sprite>(filepath, color, autoSize, size, shaderName);

	if (screenSystemRef) {
		Screen* s = screenSystemRef->getCurrentScreen();
		if (s) {
			s->registerEntity(entity);
		}
	}

	return entity;
}

ECS::Entity* EntityManager::createTextLabelEntity(glm::vec2 position, float rotation, float scale, const std::string& text, const std::string& font, glm::vec3 color) {
	ECS::Entity* entity = world->create();
	entity->assign<Transform>(position, rotation, scale);
	entity->assign<TextLabel>(text, font, color);

	if (screenSystemRef) {
		Screen* s = screenSystemRef->getCurrentScreen();
		if (s) {
			s->registerEntity(entity);
		}
	}

	return entity;
}

void EntityManager::createInvocation(glm::vec2 position, int coliderId, int attack, const char* text_back, const char* text_inv, const char* text_att, int playerOwner, TextRenderer* tr) {
	glm::vec2 defaultSize(61.f, 92.f);
	glm::vec2 invocSize = defaultSize;
	std::string displayName =  "";

	const InvocationData* invData = nullptr;
	for (int i = 0; i < Invocations::numInvocations; ++i) {
		if (strcmp(Invocations::data[i].texturePath, text_inv) == 0) {
			invData = &Invocations::data[i];
			break;
		}
	}

	if (invData) {
		invocSize = invData->size;
		displayName = invData->name;
	}

	ECS::Entity* spawn_card = createEntityWithoutScript(glm::vec2(0.f, 10.f), 0.f, 1.f, text_back, glm::vec3(1., 1., 1.), false, glm::vec2(150., 170.));

	auto result_gobobo = createEntity(position, 0.f, 1.f, text_inv, glm::vec3(1., 1., 1.), false, invocSize, "menu_motion", "InvocationScript");
	ECS::Entity* spawn_gobobo = result_gobobo.first;

	if (invData) {
		auto sprite = spawn_gobobo->get<Sprite>();
		sprite->ampScale = invData->idle.ampScale;
		sprite->ampAngle = invData->idle.ampAngle;
		sprite->freq = invData->idle.freq;
		sprite->phase = invData->idle.phase;
	}

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

	ECS::Entity* att = createEntityWithoutScript(glm::vec2(0.f, 60.f), 0.f, 1.f, text_att, glm::vec3(1., 1., 1.), false, glm::vec2(12., 32.));

	invoc_script->setInvocRefs(spawn_gobobo, spawn_card, att);

	spawn_card->get<Transform>()->assignParent(&spawn_gobobo->get<Transform>().get());
	att->get<Transform>()->assignParent(&spawn_gobobo->get<Transform>().get());

	if (!displayName.empty()) {
		auto* labelEnt = createTextLabelEntity(
			glm::vec2(0.f, - 60.f),
			0.f,
			0.35f,           // aquí va a Transform::scale
			displayName,
			"ActionMan",
			glm::vec3(0.f)   // negro
		);

		// Parentamos la etiqueta para heredar transform
		labelEnt->get<Transform>()->assignParent(&spawn_gobobo->get<Transform>().get());

		labelEnt->get<Transform>()->layer = 5;
	}
}

void EntityManager::createInvocationWithPassive(glm::vec2 position, int coliderId, int attack, const char* text_back, const char* text_inv, const char* text_att, int playerOwner, PassiveAbility* passive, TextRenderer* tr) {
	createInvocation(position, coliderId, attack, text_back, text_inv, text_att, playerOwner, tr);

	InvocationScript* latestInvocationScript = gameManagerRef->getInvocationScripts().back();
	latestInvocationScript->setPassiveAbility(passive); 

}

ECS::Entity* EntityManager::createCell(glm::vec2 position, const char* text_cell_type, float col_w, float col_h, bool conquerable) {
	auto result_cell = createEntity(position, 0.f, 1.f, text_cell_type, glm::vec3(1., 1., 1.), true, glm::vec2(1.0F, 1.0F), "default", "CellScript");
	ECS::Entity* cell = result_cell.first;
	Script* generic_script = result_cell.second;

	CellScript* cell_script = dynamic_cast<CellScript*>(generic_script);
	if (!cell_script) {
		std::cerr << "Error: Failed to assign CellScript!" << std::endl;
		return nullptr;
	}

	cell->assign<BoxCollider>(col_w, col_h, 1);
	gameManagerRef->addCellToList(cell_script);
	cell_script->setCellRef(cell);
	cell_script->setConquerable(conquerable);

	return cell;
}

void EntityManager::destroyEntity(ECS::Entity* entity) {
	if (!entity) return;

	std::cout << "[EntityManager] Destroying entity at address: " << entity << std::endl;

	// 1) Localizar el Transform que estamos a punto de destruir (si existe)
	Transform* rawTransformPtr = nullptr;
	if (auto t = entity->get<Transform>()) {
		// Sacamos la dirección cruda
		rawTransformPtr = &t.get();
	}

	// 2) Para cada Transform en el mundo, si parent == rawTransformPtr, anularlo:
	if (rawTransformPtr) {
		world->each<Transform>([&](ECS::Entity* otherEnt, ComponentHandle<Transform> otherT) {
			if (otherT->parent == rawTransformPtr) {
				otherT->parent = nullptr;
			}
			});
	}

	if (screenSystemRef) {
		OverlaySystem* om = screenSystemRef->getOverlaySystem();
		if (om) {
			om->removeEntityFromAllGroups(entity);
		}
	}

	// 3) Now inform the current Screen so that its internal bookkeeping
	//    (the managedOverlayEntities vector of (groupName, ent) pairs) is cleaned up too.
	if (screenSystemRef) {
		Screen* s = screenSystemRef->getCurrentScreen();
		if (s) {
			// Each Screen knows exactly which groupName each entity was registered under,
			// so we can iterate over that stored list and unregister each pair.
			// However, we do not have direct access here to s->managedOverlayEntities,
			// so we’ll rely on s->unregisterOverlayEntity(...) being called once per groupName.
			//
			// We can either:
			//  A) Add a Screen method `unregisterFromAllGroups(entity)` that itself
			//     loops over each (groupName, ent) pair in managedOverlayEntities and calls
			//     unregisterOverlayEntity(groupName, ent), or
			//  B) Expose a public getter of managedOverlayEntities. We’ll show option (A) here:
			s->unregisterOverlayEntityFromAllGroups(entity);
		}
	}

	// 4) Also call the generic “unregisterEntity” so that any non?overlay registration is cleaned:
	if (screenSystemRef) {
		Screen* s = screenSystemRef->getCurrentScreen();
		if (s) {
			s->unregisterEntity(entity);
		}
	}

	// 4) Destruir la entidad en el world
	world->destroy(entity);
}


void EntityManager::setManagerRefs(GameManager* gmr, ButtonManager* bmr, ScriptManager* smr, ScreenSystem* smrRef) {
	gameManagerRef = gmr;
	buttonManagerRef = bmr;
	scriptManagerRef = smr;
	screenSystemRef = smrRef;

}

ButtonManager* EntityManager::getButtonManagerRef() {
	return buttonManagerRef;
}

GameManager* EntityManager::getGameManagerRef() {
	return gameManagerRef;
}

void EntityManager::settingCardsSelectionList(InvocationSelectionScript* invssc) {
	gameManagerRef->addInvocationSelectionScriptToList(invssc);
}