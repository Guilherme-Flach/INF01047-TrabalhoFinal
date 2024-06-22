#include "engine/EngineObject/engineObject.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <vector>

EngineObject::EngineObject(glm::vec4 position) : parent(nullptr) {
    this->model = Matrix_Identity();
    this->model[3] = position;
}

std::vector<EngineObject> EngineObject::get_children() { return children; }
EngineObject *EngineObject::get_parent() { return parent; }
glm::vec4 EngineObject::get_position() { return model[3]; }

void EngineObject::set_parent(EngineObject &parent) { this->parent = &parent; }
void EngineObject::set_position(glm::vec4 position) {
    this->model[3] = position;
}

void EngineObject::addChild(EngineObject &child) {
    child.set_parent(*this);
    children.push_back(child);
}

void EngineObject::translate(glm::vec4 offset) { this->model[3] += offset; }

void EngineObject::rotate(glm::vec3 rotation) {
    this->model = Matrix_Rotate_Z(rotation.z) * Matrix_Rotate_Y(rotation.y) *
                  Matrix_Rotate_X(rotation.x) * model;
}

glm::vec4 EngineObject::get_global_position() {
    glm::vec4 new_position = this->model[3];
    EngineObject *current_node = this->parent;
    while (current_node != nullptr) {
        new_position = Matrix_ToParentCoordinates(current_node->model[3],
                                                  {current_node->model[0],
                                                   current_node->model[1],
                                                   current_node->model[2]}) *
                       new_position;
        current_node = current_node->parent;
    }
    return new_position;
}

glm::mat4 EngineObject::get_model_matrix() {
    auto model = this->model;
    EngineObject *node = this;
    while ((node = node->parent) != nullptr)
        model = node->model * model;
    return model;
}

glm::mat3x4 EngineObject::get_global_basis() {
    EngineObject *current_node = this;
    glm::mat3x4 current_basis = {current_node->model[0], current_node->model[1],
                                 current_node->model[2]};
    while ((current_node = current_node->parent) != nullptr)
        current_basis = Matrix_PopBasis(
            current_basis, {current_node->model[0], current_node->model[1],
                            current_node->model[2]});
    return current_basis;
}

glm::mat3x4 EngineObject::get_basis() {
    return {this->model[0], this->model[1], this->model[2]};
}
