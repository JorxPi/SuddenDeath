#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

using namespace std;

class SpriteRenderer
{
public:
    SpriteRenderer();

    void Init();

    void SetTime(float time);

    void DrawSprite(Texture& texture, glm::mat4 proj, glm::vec2 position,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f), std::string shaderName = "default",
        float atac1 = 0.0f, float atac2 = 0.0f,
        glm::vec3 ruletaColor1 = glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3 ruletaColor2 = glm::vec3(0.0f, 1.0f, 0.0f));

    void Delete();
private:
    std::shared_ptr<Shader> shaderDefault;
    std::shared_ptr<Shader> shaderInverted;
    std::shared_ptr<Shader> shaderRepeating;
    std::shared_ptr<Shader> shaderRuleta;
    std::shared_ptr<VAO> vao;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<EBO> ebo;

    float t = 0;
};