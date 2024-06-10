#include "engine/gameObject.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engine/loader.hpp"
#include "engine/renderObject.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "matrices.hpp"

#include <vector>

GameObject::GameObject(glm::vec4 position, GLuint program_id)
    : program_id(program_id), position(position) {}

glm::mat4 GameObject::get_model_matrix() {
    glm::mat4 parentTransform = Matrix_Identity();
    if (parent != nullptr) {
        parentTransform = parent->get_model_matrix();
    }

    return parentTransform *
           Matrix_Translate(position.x, position.y, position.z) *
           Matrix_Scale(modelScaling.x, modelScaling.y, modelScaling.z) *
           Matrix_Rotate_Z(rotation.z) * Matrix_Rotate_Y(rotation.y) *
           Matrix_Rotate_X(rotation.x) * Matrix_Identity();
}

std::vector<GameObject> GameObject::get_children() { return children; }
RenderObject *GameObject::get_model() { return model; }
glm::vec3 GameObject::get_modelScaling() { return modelScaling; }
GameObject *GameObject::get_parent() { return parent; }
glm::vec4 GameObject::get_position() { return position; }
glm::vec3 GameObject::get_rotation() { return rotation; }

void GameObject::set_model(RenderObject *model) { this->model = model; }
void GameObject::set_modelScaling(glm::vec3 modelScaling) {
    this->modelScaling = modelScaling;
}
void GameObject::set_parent(GameObject *parent) { this->parent = parent; }
void GameObject::set_position(glm::vec4 position) { this->position = position; }
void GameObject::set_rotation(glm::vec3 rotation) { this->rotation = rotation; }

void GameObject::addChild(GameObject *child) {
    child->set_parent(this);
    children.push_back(*child);
}

void GameObject::translate(glm::vec4 offset) { position += offset; }

void GameObject::rotate(glm::vec3 rotation) { this->rotation += rotation; }

void GameObject::render() {
    for (GameObject child : children) {
        child.render();
    }
    GLint model_uniform = glGetUniformLocation(program_id, "model");
    glUniformMatrix4fv(model_uniform, 1, GL_FALSE,
                       glm::value_ptr(get_model_matrix()));
    model->render();
}
