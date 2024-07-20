#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"

PhysicsObject::PhysicsObject(glm::vec4 position, GLfloat mass)
    : GameObject(position), drag(0), mass(mass),
      velocity({0.0f, 0.0f, 0.0f, 0.0f}),
      acceleration({0.0f, 0.0f, 0.0f, 0.0f}),
      angularVelocity({0.0f, 0.0f, 0.0f}) {}

void PhysicsObject::accelerate(glm::vec4 velocity) {
    this->velocity += velocity;
}

void PhysicsObject::increase_acceleration(glm::vec4 acceleration) {
    this->acceleration += acceleration;
}

void PhysicsObject::increase_angularVelocity(glm::vec3 angularVelocity) {
    this->angularVelocity += angularVelocity;
}

void PhysicsObject::applyForce(glm::vec4 force) { velocity += force * mass; }

void PhysicsObject::physicsUpdate(GLfloat deltaTime) {
    velocity += acceleration;
    float velocityNorm = norm(velocity);
    // Keep velocity bound inside of the limits
    if (velocityNorm > speedLimit) {
        velocity = speedLimit * (velocity / velocityNorm);
    }
    translate(velocity * deltaTime);

    for (auto child : children) {
        child->rotate(angularVelocity * deltaTime);
    }

    // Pseudo drag
    velocity -= velocity * drag * deltaTime;
}
