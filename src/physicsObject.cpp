#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"

PhysicsObject::PhysicsObject(glm::vec4 position, GLfloat mass) : GameObject(position), mass(mass), speed({0.0f, 0.0f, 0.0f, 0.0f}), acceleration({0.0f, 0.0f, 0.0f, 0.0f}), angularSpeed({0.0f, 0.0f, 0.0f}), drag(0) { }

void PhysicsObject::accelerate(glm::vec4 speed) {
    this->speed += speed;
}

void PhysicsObject::increase_acceleration(glm::vec4 acceleration){
    this->acceleration += acceleration;
}

void PhysicsObject::increase_angularSpeed(glm::vec3 speed){
    this->angularSpeed += speed;
}


void PhysicsObject::applyForce(glm::vec4 force) {
    speed += force * mass;
}

void PhysicsObject::update(GLfloat deltaTime) {
    speed += acceleration;
    float speedNorm = norm(speed);
    // Keep speed bound inside of the limits
    if (speedNorm > speedLimit) {
        speed = speedLimit * (speed / speedNorm);
    }
    translate(speed * deltaTime);

    for (auto child: children) {
        child->rotate(angularSpeed * deltaTime);
    }

    // Pseudo drag
    speed -= speed * drag * deltaTime;
}



