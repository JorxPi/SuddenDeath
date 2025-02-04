#include "SpriteRenderer.h"

// Vertices coordinates
GLfloat vertices[] =
{ // COORDINATES  /  TexCoord //
    -1.f, -1.f, 0.0f, 1.0f, // Lower left corner
    -1.f,  1.f, 0.0f, 0.0f, // Upper left corner
     1.f,  1.f, 1.0f, 0.0f, // Upper right corner
     1.f, -1.f, 1.0f, 1.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
    0, 2, 1, // Upper triangle
    0, 3, 2 // Lower triangle
};

SpriteRenderer::SpriteRenderer() 
{
    Init();
}

void SpriteRenderer::Init()
{
    shaderDefault = std::make_shared<Shader>("default.vert", "default.frag");
    shaderInverted = std::make_shared<Shader>("default.vert", "inverted.frag");
    shaderRepeating = std::make_shared<Shader>("default.vert", "repeating.frag");
    shaderRuleta = std::make_shared<Shader>("ruleta.vert", "ruleta.frag");

    // Generates Vertex Array Object and binds it
    vao = std::make_shared<VAO>();
    vao->Bind();

    // Generates Vertex Buffer Object and links it to vertices
    vbo = std::make_shared<VBO>(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    ebo = std::make_shared<EBO>(indices, sizeof(indices));

    // Links VBO to VAO
    vao->LinkAttrib(*vbo, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
    vao->LinkAttrib(*vbo, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    vao->Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

//,float atac1 = 0.0f, float atac2 = 0.0f, glm::vec3 ruletaColor1 = glm::vec3(1.0f), glm::vec3 ruletaColor2 = glm::vec3(1.0f))

void SpriteRenderer::DrawSprite(Texture& texture, glm::mat4 proj, glm::vec2 position,
    glm::vec2 size, float rotate, glm::vec3 color, std::string shaderName,
    float atac1, float atac2,
    glm::vec3 ruletaColor1, glm::vec3 ruletaColor2)
{
    std::shared_ptr<Shader> shader = shaderDefault;
    if (shaderName == "inverted") {
        shader = shaderInverted;
    }
    if (shaderName == "repeating") {
        shader = shaderRepeating;
    }
    if (shaderName == "ruleta") {
        shader = shaderRuleta;
        shader->Activate(); // Make sure this is done before setting uniforms
        shader->SetFloat("atac1", atac1);
        shader->SetFloat("atac2", atac2);
        shader->SetVector3f("ruletaColor1", ruletaColor1);
        shader->SetVector3f("ruletaColor2", ruletaColor2);
    }
    
    // prepare transformations
    shader->Activate();
    glm::mat4 model = glm::mat4(1.0f);

    // glm::vec2 actuall position = position
    // Transform* current_transform = 


    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size/2.0f, 1.0f));

    shader->SetMatrix4("model", model);
    shader->SetMatrix4("proj", proj);
    shader->SetVector3f("tint", color);
    shader->SetFloat("t", t);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    vao->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    vao->Unbind();
}

void SpriteRenderer::SetTime(float time) {  
    t = time;
}

void SpriteRenderer::Delete() {
    vao->Delete();
    vbo->Delete();
    ebo->Delete();

    shaderDefault->Delete();
    shaderInverted->Delete();
}
