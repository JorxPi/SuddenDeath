    #include "ButtonScript.h"
    #include <functional>

    //GENERAL LOGIC



    void ButtonScript::tickScript(float deltaTime) {
        ComponentHandle<Transform> transform = entity->get<Transform>();
        ComponentHandle<BoxCollider> collider = entity->get<BoxCollider>();
        ComponentHandle<Sprite> sprite = entity->get<Sprite>();

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // 2) simple AABB hover test
        bool over =
            mouseX >= transform->position.x - collider->width * 0.5f &&
            mouseX <= transform->position.x + collider->width * 0.5f &&
            mouseY >= transform->position.y - collider->height * 0.5f &&
            mouseY <= transform->position.y + collider->height * 0.5f;

        // 3) immediately switch shader and hover amount
        if (over) {
            sprite->shaderName = "hover";
        }
        else {
            sprite->shaderName = "default";
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

            if (mouseX >= transform->position.x - collider->width / 2.0f && mouseX <= transform->position.x + collider->width / 2.0f &&
                mouseY >= transform->position.y - collider->height / 2.0f && mouseY <= transform->position.y + collider->height / 2.0f) {
                if (onClickAction) {
                    onClickAction();
                }
            }
        }
    }

    void ButtonScript::setOnClickAction(std::function<void()> action) {
        onClickAction = action;
    }



    //GETTERS



    Entity* ButtonScript::getEntity() const { 
        return entity; 
    }
