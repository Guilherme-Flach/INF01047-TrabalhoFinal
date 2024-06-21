#include "engine/EngineObject/engineObject.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <vector>

EngineObject::EngineObject(glm::vec4 position) : position(position) {}

std::vector<EngineObject> EngineObject::get_children() { return children; }
EngineObject *EngineObject::get_parent() { return parent; }
glm::vec4 EngineObject::get_position() { return position; }


void EngineObject::set_parent(EngineObject *parent) { this->parent = parent; }
void EngineObject::set_position(glm::vec4 position) { this->position = position; }

void EngineObject::addChild(EngineObject *child) {
    child->set_parent(this);
    children.push_back(*child);
}

void EngineObject::translate(glm::vec4 offset) { position += offset; }