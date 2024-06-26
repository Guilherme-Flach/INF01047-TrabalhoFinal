#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"

PhysicsObject::PhysicsObject(glm::vec4 position, GLfloat mass) : GameObject(position), mass(mass), speed({0.0f, 0.0f, 0.0f, 0.0f}) { }

void PhysicsObject::accelerate(glm::vec4 speed) {
    this->speed += speed;
}

void PhysicsObject::applyForce(glm::vec4 force) {
    speed += force * mass;
}

void PhysicsObject::update(GLfloat deltaTime) {
    float speedNorm = norm(speed);
    // Keep speed bound inside of the limits
    if (speedNorm > speedLimit) {
        speed = speedLimit * (speed / speedNorm);
    }
    translate(speed * deltaTime);
}



