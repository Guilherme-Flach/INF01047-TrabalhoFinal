#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"
#include "engine/Physics/player.hpp"
#include <iostream>

GLfloat Player::playerBaseSpeed = 0.5f;
GLfloat Player::playerBoostSpeed = 2.0f;
GLfloat Player::cameraRotationSpeed = 1.5f;

void Player::update(GLfloat deltaTime) {
    {
        const float playerSpeed = isBoosting? playerBoostSpeed : playerBaseSpeed;
        
        const glm::vec4 axis = ship->get_w_vector();
        ship->rotate(rotationRate.x * deltaTime * cameraRotationSpeed, ship->get_u_vector());
        ship->rotate(rotationRate.y * deltaTime * cameraRotationSpeed, ship->get_v_vector());
        ship->rotate(rotationRate.z * deltaTime * cameraRotationSpeed, ship->get_w_vector());
        ship->set_up_vector(ship->get_global_basis()[1]);

        // Update camera
        const glm::vec4 movementLateral  = movement.x * ship->get_u_vector();
        const glm::vec4 movementVertical = movement.y * ship->get_v_vector();
        const glm::vec4 movementFrontal  = movement.z * ship->get_w_vector();
        const glm::vec4 movementTotal    = movementFrontal + movementVertical + movementLateral;
        speed += movementTotal * deltaTime * playerSpeed;
        camera->rotate(rotationRate.x * deltaTime * cameraRotationSpeed, ship->get_u_vector());
        camera->rotate(rotationRate.y * deltaTime * cameraRotationSpeed, ship->get_v_vector());
        camera->rotate(rotationRate.z * deltaTime * cameraRotationSpeed, ship->get_w_vector());
        camera->set_up_vector(ship->get_up_vector());
        
        
        PhysicsObject::update(deltaTime);
    };
}