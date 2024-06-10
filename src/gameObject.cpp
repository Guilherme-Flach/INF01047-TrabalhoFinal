#include "engine/gameObject.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engine/RenderObject.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "matrices.hpp"

#include <vector>

GameObject::GameObject(glm::vec4 position) : position(position) {}

glm::mat4 GameObject::get_model_matrix() {
    glm::mat4 parentTransform = Matrix_Identity();
    if (parent != nullptr) {
        parentTransform = parent->get_model_matrix();
    } 

    return parentTransform * Matrix_Translate(position.x, position.y, position.z) *
           Matrix_Scale(modelScaling.x, modelScaling.y, modelScaling.z) *
           Matrix_Rotate_Z(rotation.z) * Matrix_Rotate_Y(rotation.y) *
           Matrix_Rotate_X(rotation.x) * Matrix_Identity();
}

glm::vec4 GameObject::get_position() { return position; }

void GameObject::set_position(glm::vec4 position) { this->position = position; }

void GameObject::set_model(RenderObject *model) { this->model = model; }

void GameObject::addChild(GameObject child) { children.push_back(child); }

void GameObject::translate(glm::vec4 offset) { position += offset; }

void GameObject::rotate(glm::vec3 rotation) { this->rotation += rotation; }

void GameObject::render(int program_id) {
    for(GameObject child : children) {
        child.render(program_id);
    }
    GLint model_uniform = glGetUniformLocation(program_id, "model");
    glUniformMatrix4fv(model_uniform, 1, GL_FALSE,
                    glm::value_ptr(get_model_matrix()));
    model->render();
}
