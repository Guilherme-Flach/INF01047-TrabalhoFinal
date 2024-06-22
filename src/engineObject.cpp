#include "engine/EngineObject/engineObject.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <vector>

EngineObject::EngineObject(glm::vec4 position)
    : EngineObject(position, nullptr) {}
EngineObject::EngineObject(glm::vec4 position, EngineObject *parent)
    : parent(parent) {
    if (parent == nullptr) {
        this->basis = Matrix_ToBasis(Matrix_Identity());
        return;
    }
    this->basis = parent->basis;
}

std::vector<EngineObject> EngineObject::get_children() { return children; }
EngineObject *EngineObject::get_parent() { return parent; }
glm::vec4 EngineObject::get_position() { return position; }

void EngineObject::set_parent(EngineObject *parent) { this->parent = parent; }
void EngineObject::set_position(glm::vec4 position) {
    this->position = position;
}

void EngineObject::addChild(EngineObject *child) {
    child->set_parent(this);
    children.push_back(*child);
}

void EngineObject::translate(glm::vec4 offset) { position += offset; }

void EngineObject::rotate(float angle_x, float angle_y, float angle_z) {
    if (angle_x != 0)
        this->basis.x = Matrix_Rotate_X(angle_x) * this->basis.x;
    if (angle_y != 0)
        this->basis.y = Matrix_Rotate_Y(angle_y) * this->basis.y;
    if (angle_z != 0)
        this->basis.z = Matrix_Rotate_Z(angle_z) * this->basis.z;
}

glm::vec4 EngineObject::get_global_position() {
    glm::vec4 new_position = this->position;
    EngineObject *current_node = this;
    while (current_node != nullptr) {
        new_position = Matrix_ToParentCoordinates(current_node->position,
                                                  current_node->basis) *
                       new_position;
        current_node = current_node->parent;
    }
    return new_position;
}
