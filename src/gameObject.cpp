#include "engine/EngineObject/gameObject.hpp"
#include "engine/Rendering/defaultModels.hpp"
#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <vector>

GameObject::GameObject(glm::vec4 position) : model(nullptr), parent(nullptr) {
    static Model3D default_model = BaseAxesModel();
    this->model_matrix = Matrix_Identity();
    this->model_matrix[3] = position;
    this->model = &default_model;
}

std::vector<GameObject *> *GameObject::get_children() { return &children; }
GameObject *GameObject::get_parent() { return parent; }
glm::vec4 GameObject::get_position() { return model_matrix[3]; }

void GameObject::set_parent(GameObject &parent) { this->parent = &parent; }
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
glm::vec3 GameObject::get_modelScaling() { return modelScaling; }

bool GameObject::get_isRenderable() { return isRenderable; }

void GameObject::set_model(Model3D &model) {
    this->model = &model;
    isRenderable = true;
}
void GameObject::set_modelScaling(glm::vec3 modelScaling) {
    this->modelScaling = modelScaling;
}
