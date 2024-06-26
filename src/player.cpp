#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"
#include "engine/Physics/player.hpp"

const GLfloat Player::playerSpeed = 0.5f;
const GLfloat Player::cameraRotationSpeed = 0.5f;

void Player::update(GLfloat deltaTime) {
    {
        const glm::vec4 movementLateral  = movement.x * camera->get_u_vector();
        const glm::vec4 movementVertical = movement.y * camera->get_v_vector();
        const glm::vec4 movementFrontal  = movement.z * camera->get_w_vector();
        const glm::vec4 movementTotal    = movementFrontal + movementVertical + movementLateral;

        speed += movementTotal * deltaTime * playerSpeed;
        camera->increase_phi(rotationRate.x * deltaTime * cameraRotationSpeed);
        camera->increase_theta( rotationRate.y * deltaTime * cameraRotationSpeed);
        const glm::vec4 axis = camera->get_w_vector();
        camera->set_up_vector(Matrix_Rotate(rotationRate.z * deltaTime * cameraRotationSpeed, axis) * camera->get_up_vector());
        PhysicsObject::update(deltaTime);
    };
}