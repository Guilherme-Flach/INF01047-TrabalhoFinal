#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/ship.hpp"
#include "engine/Rendering/renderer.hpp"
#include "matrices.hpp"

const GLfloat Ship::shipBaseSpeed = 5.0f;
const GLfloat Ship::shipBoostingSpeed = 10.0f;
const GLfloat Ship::turningSpeed = 1.5f;
const GLfloat Ship::shipDrag = 0.5f;

Ship::Ship(glm::vec4 position)
    : PhysicsObject(position, playerMass),
      shipContainer(
          GameObject(GameObjectType::STANDARD, {0.0f, 0.2f, 0.0f, 1.0f})),
      shipCollider(SphereCollider(this, {0.0, 0.0, 0.0, 1.0}, 2.0f)) {
    set_drag(shipDrag);
    addChild(shipContainer);
    shipContainer.set_texture(
        Renderer::instance().loadTexture("ship", "../../data/ship/ship.jpeg"));
    shipContainer.set_model(
        Renderer::instance().loadModel("ship", "../../data/ship/ship.obj"));
    Renderer::instance().addToRenderQueue(Renderer::RenderMode::PHONG,
                                          &shipContainer);
    shipContainer.set_modelScaling(0.6f);
    addChild(shipCollider);
};

void Ship::physicsUpdate(GLfloat deltaTime) {
    {
        const float movementSpeed =
            isBoosting ? shipBoostingSpeed : shipBaseSpeed;

        const glm::mat3x4 basis = shipContainer.get_global_basis();

        shipContainer.rotate(rotationRate.x * deltaTime * turningSpeed,
                             basis[0]);
        shipContainer.rotate(rotationRate.y * deltaTime * turningSpeed,
                             basis[1]);
        shipContainer.rotate(rotationRate.z * deltaTime * turningSpeed,
                             basis[2]);

        const glm::vec4 movementLateral = thrusters.x * basis[0];
        const glm::vec4 movementVertical = thrusters.y * basis[1];
        const glm::vec4 movementFrontal = thrusters.z * basis[2];
        const glm::vec4 movementTotal =
            movementFrontal + movementVertical + movementLateral;
        applyForce(movementTotal * deltaTime * movementSpeed);

        PhysicsObject::physicsUpdate(deltaTime);
    };
}
