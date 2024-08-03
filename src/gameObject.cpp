#include "engine/EngineObject/gameObject.hpp"
#include "engine/Rendering/defaultModels.hpp"
#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include <functional>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <vector>

GameObject::GameObject(GameObjectType type, glm::vec4 position)
    : type(type), parent(nullptr), texture(nullptr), model(nullptr),
      modelScaling({1.0f, 1.0f, 1.0f}) {
    // static Model3D default_model = BaseAxesModel();
    this->onUpdate = [](GLfloat _) -> void {};
    this->model_matrix = Matrix_Identity();
    this->model_matrix[3] = position;
    this->model = nullptr; //&default_model;
}

std::vector<GameObject *> *GameObject::get_children() { return &children; }
GameObject *GameObject::get_parent() { return parent; }
glm::vec4 GameObject::get_position() { return model_matrix[3]; }

void GameObject::set_parent(GameObject &parent) { this->parent = &parent; }
void GameObject::remove_parent() { this->parent = nullptr; }
void GameObject::set_position(glm::vec4 position) {
    this->model_matrix[3] = position;
}

void GameObject::addChild(GameObject &child) {
    child.set_parent(*this);
    children.push_back(&child);
}

void GameObject::translate(glm::vec4 offset) {
    this->model_matrix[3] += offset;
}

void GameObject::rotate(glm::vec3 rotation) {
    this->model_matrix = Matrix_Rotate_Z(rotation.z) *
                         Matrix_Rotate_Y(rotation.y) *
                         Matrix_Rotate_X(rotation.x) * model_matrix;
}

void GameObject::rotate(float angle, glm::vec4 axis) {
    this->model_matrix = Matrix_Rotate(angle, axis) * model_matrix;
}

glm::vec4 GameObject::get_global_position() {
    glm::vec4 new_position = this->model_matrix[3];
    GameObject *current_node = this->parent;
    while (current_node != nullptr) {
        new_position =
            Matrix_ToParentCoordinates(current_node->model_matrix[3],
                                       {current_node->model_matrix[0],
                                        current_node->model_matrix[1],
                                        current_node->model_matrix[2]}) *
            new_position;
        current_node = current_node->parent;
    }
    return new_position;
}

glm::mat4 GameObject::get_model_matrix() {
    auto model = this->model_matrix;
    GameObject *node = this;
    while ((node = node->parent) != nullptr)
        model = node->model_matrix * model;
    return model * Matrix_Scale(modelScaling.x, modelScaling.y, modelScaling.z);
}

glm::mat3x4 GameObject::get_global_basis() {
    GameObject *current_node = this;
    glm::mat3x4 current_basis = {current_node->model_matrix[0],
                                 current_node->model_matrix[1],
                                 current_node->model_matrix[2]};
    while ((current_node = current_node->parent) != nullptr)
        current_basis =
            Matrix_PopBasis(current_basis, {current_node->model_matrix[0],
                                            current_node->model_matrix[1],
                                            current_node->model_matrix[2]});
    return current_basis;
}

glm::mat3x4 GameObject::get_basis() {
    return {this->model_matrix[0], this->model_matrix[1],
            this->model_matrix[2]};
}

Model3D *GameObject::get_model() { return model; }
Texture *GameObject::get_texture() { return texture; }
glm::vec3 GameObject::get_modelScaling() { return modelScaling; }

bool GameObject::get_isRenderable() {
    return model != nullptr && texture != nullptr;
}

void GameObject::set_model(Model3D *model) { this->model = model; }

void GameObject::set_texture(Texture *texture) { this->texture = texture; }

void GameObject::set_modelScaling(float modelScaling) {
    this->modelScaling = {modelScaling, modelScaling, modelScaling};
}

void GameObject::set_modelScaling(glm::vec3 modelScaling) {
    this->modelScaling = modelScaling;
}

void GameObject::set_onUpdate(
    std::function<void(GLfloat deltaTime)> updateFunction) {
    this->onUpdate = updateFunction;
}

void GameObject::update(GLfloat deltaTime) { this->onUpdate(deltaTime); }

GameObjectType GameObject::get_type() { return this->type; }
