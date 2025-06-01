#pragma once

#include "ScriptManager.h"
#include "ScriptSystem.h"
#include "ScriptAssigner.h"
#include "GameManager.h"
#include "ButtonManager.h"
#include "InvocationSelectionScript.h"
#include "PassiveAbilities.h"

class GameManager;

class ButtonManager;

class InvocationSelectionScript;

class ScreenManager;

class EntityManager {

public:
    EntityManager(GLFWwindow* window, ECS::World* world, ScriptSystem* scriptSystem);

    std::pair<ECS::Entity*, Script*> createEntity(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "default", const std::string& scriptName = "");
    ECS::Entity* createEntityWithoutScript(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "default");
    void createInvocation(glm::vec2 position, int coliderId, int attack, const char* text_back, const char* text_inv, const char* text_att, int playerOwner);
    void createInvocationWithPassive(glm::vec2 position, int coliderId, int attack, const char* text_back, const char* text_inv, const char* text_att, int playerOwner, PassiveAbility* passive);
    ECS::Entity* createCell(glm::vec2 position, const char* text_cell_type, float col_w, float col_h, bool conquerable);

    void destroyEntity(Entity* entity);

    void setManagerRefs(GameManager* gmr, ButtonManager* bmr, ScriptManager* smr, ScreenManager* smrRef);

    ButtonManager* getButtonManagerRef();
    GameManager* getGameManagerRef();

    void settingCardsSelectionList(InvocationSelectionScript* invssc);

private:
    GLFWwindow* window;
    ECS::World* world;
    ScriptSystem* scriptSystem;

    GameManager* gameManagerRef;
    ButtonManager* buttonManagerRef;
    ScriptManager* scriptManagerRef;
    ScreenManager* screenManagerRef = nullptr;
};
