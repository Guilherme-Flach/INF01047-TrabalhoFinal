#include "engine/Rendering/renderer.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>

Renderer::Renderer(GLuint programId) : programId(programId) {
    this->renderModels = std::vector<Model3D>();

    this->model_uniform = glGetUniformLocation(programId, "model");
    this->view_uniform = glGetUniformLocation(programId, "view");
    this->projection_uniform = glGetUniformLocation(programId, "projection");
}

Renderer &Renderer::instance(GLuint programId) {
    static Renderer *instance = new Renderer(programId);
    return *instance;
}

void Renderer::renderGameObject(GameObject *engineObject) {
    if (engineObject != NULL) {
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE,
                           glm::value_ptr(engineObject->get_model_matrix()));
        engineObject->get_model()->render();
    }

    std::vector<GameObject *> children = *engineObject->get_children();

    for (GameObject *object : children) {
        renderGameObject(object);
    }
}

void Renderer::renderGameObjects(std::vector<GameObject *> &store) {
    for (GameObject *object : store) {
        renderGameObject(object);
    }
}
