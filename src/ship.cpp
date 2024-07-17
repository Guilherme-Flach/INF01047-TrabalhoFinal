#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"
#include "engine/Physics/ship.hpp"
#include <iostream>

GLfloat Ship::playerBaseSpeed = 0.5f;
GLfloat Ship::playerBoostSpeed = 2.0f;
GLfloat Ship::cameraRotationSpeed = 1.5f;

void Ship::update(GLfloat deltaTime) {
    {
        const float playerSpeed = isBoosting? playerBoostSpeed : playerBaseSpeed;
        const glm::mat3x4 basis = shipContainer.get_global_basis();
        shipContainer.rotate(rotationRate.x * deltaTime * cameraRotationSpeed, basis[0]);
        shipContainer.rotate(rotationRate.y * deltaTime * cameraRotationSpeed, basis[1]);
        shipContainer.rotate(rotationRate.z * deltaTime * cameraRotationSpeed, basis[2]);

        const glm::vec4 movementLateral  = thrusters.x * basis[0];
        const glm::vec4 movementVertical = thrusters.y * basis[1];
        const glm::vec4 movementFrontal  = thrusters.z * basis[2];
        const glm::vec4 movementTotal    = movementFrontal + movementVertical + movementLateral;
        speed += movementTotal * deltaTime * playerSpeed;


        // if (isPiloting) {
        //     // Update camera to follow ship
        //     camera->rotate(rotationRate.x * deltaTime * cameraRotationSpeed, ship->get_u_vector());
        //     camera->rotate(rotationRate.y * deltaTime * cameraRotationSpeed, ship->get_v_vector());
        //     camera->rotate(rotationRate.z * deltaTime * cameraRotationSpeed, ship->get_w_vector());
        //     camera->set_up_vector(ship->get_up_vector());
        // } else {
        //     if (currentPlanet != nullptr) {
        //         const glm::vec4 out_vector = this->get_global_position() - currentPlanet->get_global_position();
        //         const float distance = norm(out_vector);
        //         camera->set_up_vector(out_vector/distance);
        //         set_up_vector(out_vector/distance);
        //     }
        // }
        
        PhysicsObject::update(deltaTime);
    };
}