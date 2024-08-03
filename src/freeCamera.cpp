#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Rendering/defaultModels.hpp"

FreeCamera::FreeCamera(glm::vec4 position, glm::vec4 lookTowards)
    : Camera(position, new GameObject(GameObjectType::STANDARD, lookTowards)) {
    this->addChild(*target);
}

FreeCamera::~FreeCamera() { delete target; }
