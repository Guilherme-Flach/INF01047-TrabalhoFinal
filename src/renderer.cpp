#include "engine/Rendering/renderer.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>

Renderer::Renderer(GLuint programId) : programId(programId) {
    debugMode = false;
    this->renderModels = std::vector<Model3D>();
    this->model_uniform = glGetUniformLocation(programId, "model");
    this->view_uniform = glGetUniformLocation(programId, "view");
    this->projection_uniform = glGetUniformLocation(programId, "projection");
    this->objectid_uniform =  glGetUniformLocation(programId, "object_id");

}

Renderer &Renderer::instance(GLuint programId) {
    static Renderer *instance = new Renderer(programId);
    return *instance;
}

void Renderer::setDebugMode(bool debugMode) { this->debugMode = debugMode; }

void Renderer::renderGameObject(GameObject *gameObject) {
    if (gameObject != NULL && (gameObject->get_isRenderable() || debugMode)) {
        glUniform1i(objectid_uniform, -1);
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE,
                           glm::value_ptr(gameObject->get_model_matrix()));
        gameObject->get_model()->render();
    }

    std::vector<GameObject *> children = *gameObject->get_children();

    for (GameObject *object : children) {
        renderGameObject(object);
    }
}

void Renderer::renderGameObjects(std::vector<GameObject *> &store) {
    for (GameObject *object : store) {
        renderGameObject(object);
    }
}
