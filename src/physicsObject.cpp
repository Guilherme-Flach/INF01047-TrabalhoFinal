#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/collider.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include <iostream>

const float PhysicsObject::G_CONSTANT = 0.001f;
const GLfloat PhysicsObject::speedLimit = 20.0f;
const GLfloat PhysicsObject::collisionAttenuation = 0.0;

PhysicsObject::PhysicsObject(glm::vec4 position, GLfloat mass)
    : GameObject(GameObjectType::STANDARD, position), drag(0), mass(mass),
      velocity(NONE), acceleration(NONE), angularVelocity(NONE),
      previousPosition(position) {}

void PhysicsObject::accelerate(glm::vec4 velocity) {
    this->velocity += velocity;
}

void PhysicsObject::increase_acceleration(glm::vec4 acceleration) {
    this->acceleration += acceleration;
}

void PhysicsObject::increase_angularVelocity(glm::vec3 angularVelocity) {
    this->angularVelocity += angularVelocity;
}

void PhysicsObject::applyForce(glm::vec4 force) { velocity += force / mass; }

void PhysicsObject::physicsUpdate(GLfloat deltaTime) {
    velocity += acceleration * deltaTime;
    previousPosition = get_position();

    float velocityNorm = norm(velocity);

    // // Keep velocity bound inside of the limits
    // if (velocityNorm > speedLimit) {
    //     velocity = speedLimit * (velocity / velocityNorm);
    // }

    translate(velocity * deltaTime);

    for (auto child : children) {
        child->rotate(angularVelocity * deltaTime);
    }

    // Pseudo drag
    velocity -= velocity * drag * deltaTime;
}

void PhysicsObject::handle_collision(glm::vec4 collision_point,
                                     PhysicsObject &other, GLfloat deltaTime) {
    auto new_velocity_first = velocity;
    auto new_velocity_second = other.velocity;

    auto first_global_position = get_global_position();
    auto second_global_position = other.get_global_position();

    new_velocity_first +=
        (1 / mass) * Matrix_Project(other.velocity, second_global_position -
                                                        first_global_position);
    new_velocity_first -= Matrix_Project(velocity, first_global_position -
                                                       second_global_position);

    new_velocity_second +=
        (1 / other.mass) * Matrix_Project(velocity, second_global_position -
                                                        first_global_position);
    new_velocity_second -= Matrix_Project(
        other.velocity, first_global_position - second_global_position);

    auto first_acceleration = new_velocity_first - velocity;
    auto second_acceleration = new_velocity_second - other.velocity;

    accelerate(first_acceleration);
    other.accelerate(second_acceleration);

    translate(first_acceleration * deltaTime);
    other.translate(second_acceleration * deltaTime);
}
