#include "engine/Rendering/renderer.hpp"
#include "engine/EngineObject/engineObject.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "matrices.hpp"

#include <iostream>
#include <vector>

// Singleton patter renderer
Renderer::Renderer(GLuint programId) : programId(programId) {
    this->renderModels = std::vector<Model3D>();
    this->renderObjects = std::vector<GameObject *>();
    
    this->model_uniform = glGetUniformLocation(programId, "model");
    this->view_uniform = glGetUniformLocation(programId, "view");
    this->projection_uniform = glGetUniformLocation(programId, "projection");
}

Renderer& Renderer::instance(GLuint programId) {
    static Renderer *instance = new Renderer(programId);
    return *instance;
}

void Renderer::renderGameObject(EngineObject* engineObject) {
    // UGLY: Evil trick, kinda bad
    // Check if target is a GameObject or derived class (renderable)
    GameObject* castedObject = static_cast<GameObject*> (engineObject);

    if (castedObject != NULL) {
        // Render it
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE,
                        glm::value_ptr(castedObject->get_model_matrix()));
        castedObject->get_model()->render();
        
    }

    // Render its children
    std::vector<EngineObject*> children = *engineObject->get_children();
    
    for (EngineObject* object : children) {
        renderGameObject(object);
    }

}

void Renderer::renderGameObjects() {
    for (GameObject* object : this->renderObjects) {
        renderGameObject(object);
    }
    

}

void Renderer::addGameObject(GameObject *gameObject) {
    this->renderObjects.push_back(gameObject);
}