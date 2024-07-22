#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/ship.hpp"

GLfloat Ship::shipBaseSpeed = 0.5f;
GLfloat Ship::shipBoostingSpeed = 2.0f;
const GLfloat Ship::turningSpeed = 1.5f;

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
        velocity += movementTotal * deltaTime * movementSpeed;

        PhysicsObject::physicsUpdate(deltaTime);
    };
}
