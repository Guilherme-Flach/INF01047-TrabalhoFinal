#include "engine/EngineObject/gameObject.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engine/EngineObject/engineObject.hpp"
#include "engine/loader.hpp"
#include "engine/Rendering/model3D.hpp"

GameObject::GameObject(glm::vec4 position, GLuint program_id)
    : EngineObject(position) {}

Model3D *GameObject::get_model() { return model; }
glm::vec3 GameObject::get_modelScaling() { return modelScaling; }

void GameObject::set_model(Model3D *model) { this->model = model; }
void GameObject::set_modelScaling(glm::vec3 modelScaling) {
    this->modelScaling = modelScaling;
}
