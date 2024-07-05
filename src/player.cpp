#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"
#include "engine/Physics/player.hpp"
#include <iostream>

const GLfloat Player::playerSpeed = 0.5f;
const GLfloat Player::cameraRotationSpeed = 0.5f;

void Player::update(GLfloat deltaTime) {
    {
        const glm::vec4 movementLateral  = movement.x * camera->get_u_vector();
        const glm::vec4 movementVertical = movement.y * camera->get_v_vector();
        const glm::vec4 movementFrontal  = movement.z * camera->get_w_vector();
        const glm::vec4 movementTotal    = movementFrontal + movementVertical + movementLateral;

        speed += movementTotal * deltaTime * playerSpeed;
        const glm::vec4 axis = camera->get_w_vector();
        camera->set_up_vector(Matrix_Rotate(rotationRate.z * deltaTime * cameraRotationSpeed, axis) * camera->get_up_vector());
        camera->rotate(rotationRate.x * deltaTime * cameraRotationSpeed, camera->get_u_vector());
        camera->rotate(rotationRate.y * deltaTime * cameraRotationSpeed, camera->get_v_vector());
        camera->rotate(rotationRate.z * deltaTime * cameraRotationSpeed, camera->get_w_vector());
        PhysicsObject::update(deltaTime);
    };
}