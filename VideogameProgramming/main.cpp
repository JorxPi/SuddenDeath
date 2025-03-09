#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "SpriteRenderer.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ScriptManager.h"
#include "TextRenderer.h"
#include "Script.h"
#include "GameManager.h"
#include "ButtonManager.h"
#include "EntityManager.h"
#include "Ruleta.h"
#include "InvocationScript.h"
#include "CellScript.h"
#include "CellConstants.h"
#include "ScreenManager.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "StartGameScreen.h"
#include "OverlayManager.h"
#include "InputManager.h"


#include "ECS.h"

using std::cout; 
using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
using namespace ECS;

GLFWwindow* window; // Game window
const unsigned int width = 1200;
const unsigned int height = 800;

float t = 0;
time_t current_time;

World* world;
GameManager* gameManager;
ButtonManager* buttonManager;
EntityManager* entityManager;
ScreenManager* screenManager;
OverlayManager* overlayManager;
InputManager* inputManager;
TextRenderer* textRenderer;

void SetupGLFW() {

	glfwInit();

	// Tell GLFW we are using OpenGL 3.3 and the CORE profile (only the modern functions)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

bool SetupWindow() {
	//Create a GLFWwindow with size 800x800
	window = glfwCreateWindow(1200, 800, "ProgramacioVideojocs", NULL, NULL);
	if (window == NULL) {

		glfwTerminate();
		return false;
	}

	// Make window part of the current context
	glfwMakeContextCurrent(window);

	//Load GLAD and specify the viewport
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

Entity* CreateEntity(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color,
	bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "default") {
	Entity* ent = world->create();
	ent->assign<Transform>(position, rotation, scale);
	ent->assign<Sprite>(filepath, color, autoSize, size, shaderName);

	return ent;
}

void CreateInvocation(ScriptManager* scriptManager, glm::vec2 position, int coliderId, const char* text_back, const char* text_inv, const char* text_att) {
	Entity* spawn_card = CreateEntity(glm::vec2(0.f, 15.f), 0.f, 1.f, text_back, glm::vec3(1., 1., 1.), false, glm::vec2(119., 159.));

	Entity* spawn_gobobo = CreateEntity(position, 0.f, 1.f, text_inv, glm::vec3(1., 1., 1.), false, glm::vec2(61., 92.));
	spawn_gobobo->assign<BoxCollider>(61.f, 92.f, coliderId);

	InvocationScript* invoc_script = new InvocationScript(window, world, spawn_gobobo);
	spawn_gobobo->assign<ScriptComponent>(scriptManager->AddScript(invoc_script));
	gameManager->addInvocationScriptToList(invoc_script);
	invoc_script->setInvocAttack(1);
	invoc_script->setButtonManagerRef(buttonManager);
	invoc_script->setGameManagerRef(gameManager);

	Entity* att = CreateEntity(glm::vec2(0.f, 70.f), 0.f, 1.f, text_att, glm::vec3(1., 1., 1.), false, glm::vec2(14., 36.));

	invoc_script->setInvocRefs(spawn_gobobo, spawn_card, att);

	spawn_card->get<Transform>()->assignParent(&spawn_gobobo->get<Transform>().get());
	att->get<Transform>()->assignParent(&spawn_gobobo->get<Transform>().get());
}

void CreateCell(ScriptManager* scriptManager, glm::vec2 position, const char* text_cell_type, float col_w, float col_h) {
	Entity* cell = CreateEntity(position, 0.f, 1.f, text_cell_type, glm::vec3(1., 1., 1.));
	cell->assign<BoxCollider>(col_w, col_h, 1);

	CellScript* cell_script = new CellScript(window, world, cell);
	cell->assign<ScriptComponent>(scriptManager->AddScript(cell_script));
	gameManager->addCellToList(cell_script);

	cell_script->setCellRef(cell);
}

void SetupRealWorld() {


	world = World::createWorld();

	world->registerSystem(new RenderSystem(width, height));

	ScriptSystem* scriptSystem = new ScriptSystem();
	world->registerSystem(scriptSystem);

	ScriptManager* scriptManager = scriptSystem->getScriptManager();
	entityManager = new EntityManager(window, world, scriptSystem);
	buttonManager = new ButtonManager(window, world);
	gameManager = new GameManager(window, world, entityManager);
	overlayManager = new OverlayManager(entityManager);
	screenManager = new ScreenManager(window, world);
	inputManager = new InputManager(window, overlayManager);
	textRenderer = new TextRenderer(1200, 800);
	textRenderer->LoadFont("Libraries/fonts/OpenSans.ttf", 48, "OpenSans");
	textRenderer->LoadFont("Libraries/fonts/PinkChicken-Regular.ttf", 48, "PinkChicken");
	textRenderer->LoadFont("Libraries/fonts/Super Squad.ttf", 48, "SuperSquad");
	textRenderer->LoadFont("Libraries/fonts/Action Man.ttf", 48, "ActionMan");
	textRenderer->LoadFont("Libraries/fonts/KOMTXTTI.ttf", 48, "KOMTXTTI");

	gameManager->setTextRenderer(textRenderer);
	gameManager->setButtonManagerRef(buttonManager);
	gameManager->setScreenManagerRef(screenManager);
	entityManager->setManagerRefs(gameManager, buttonManager, scriptManager);
	screenManager->setManagerRefs(gameManager, overlayManager);

	screenManager->addScreen("Menu", new MenuScreen(entityManager));
	screenManager->addScreen("StartGame", new StartGameScreen(entityManager, overlayManager));

	screenManager->switchToScreen("Menu");

	scriptSystem->start(world);
}


int main() {
	
	SetupGLFW();

	if (!SetupWindow()) {
		return -1;
	}

	SetupRealWorld();

	float dt = 0;
	float time = clock();

	//Program core loop
	while (!glfwWindowShouldClose(window)) {

		// Specify the color of the background
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		//glClear(GL_COLOR_BUFFER_BIT);

		dt = clock() - time;
		time = clock();
		if (dt < 50) {
			// Specify the color of the background
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
			// Clean the back buffer and assign the new color to it
			glClear(GL_COLOR_BUFFER_BIT);

			world->tick(dt);
			if (gameManager) {
				gameManager->update(dt);
			}	
			if (screenManager) {
				screenManager->update(dt);
			}
			if (overlayManager) {
				overlayManager->update(dt);
			}
			if (inputManager) {
				inputManager->update(dt);
			}
		}

		glfwSwapBuffers(window); //Swap buffers

		// Take care of GLFW events
		glfwPollEvents();
	}

	// Cleanup

	glfwDestroyWindow(window);
	glfwTerminate();

	world->destroyWorld();

	return 0;
}