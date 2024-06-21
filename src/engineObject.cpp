#include "engine/EngineObject/engineObject.hpp"
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
    Basis default_basis;
    default_basis.x = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    default_basis.y = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    default_basis.z = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    this->basis = default_basis;
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

glm::vec4 EngineObject::get_global_position() {
    glm::vec4 new_position = this->position;
    EngineObject *current_node = this;
    EngineObject *parent = nullptr;
    while ((parent = current_node->parent) != nullptr) {
        new_position = Matrix_ChangeBasis(current_node->position,
                                          parent->position, parent->basis) *
                       new_position;
    }
    return new_position;
}
