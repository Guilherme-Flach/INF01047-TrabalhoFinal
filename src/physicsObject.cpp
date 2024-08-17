#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"

const float PhysicsObject::G_CONSTANT = 1.0f;
const GLfloat PhysicsObject::speedLimit = 100.0f;
const GLfloat PhysicsObject::collisionAttenuation = 0.0;

PhysicsObject::PhysicsObject(glm::vec4 position, GLfloat mass)
    : GameObject(GameObjectType::STANDARD, position), drag(0), mass(mass),
      velocity(NONE), acceleration(NONE), angularVelocity(NONE),
      nextPosition(position), nextVelocity(velocity),
      previousPosition(position) {}

void PhysicsObject::accelerate(glm::vec4 velocity) {
    this->nextVelocity += velocity;
}

void PhysicsObject::translate(glm::vec4 direction) {
    this->nextPosition += direction;
}

void PhysicsObject::increase_acceleration(glm::vec4 acceleration) {
    this->acceleration += acceleration;
}

void PhysicsObject::increase_angularVelocity(glm::vec3 angularVelocity) {
    this->angularVelocity += angularVelocity;
}

void PhysicsObject::applyForce(glm::vec4 force) {
    nextVelocity += force / mass;
}

void PhysicsObject::physicsUpdate(GLfloat deltaTime) {
    velocity = nextVelocity;
    set_position(nextPosition);

    velocity += acceleration * deltaTime;
    previousPosition = get_position();

    const float velocityNorm = norm(velocity);
    if (velocityNorm >= 0.001f) {
        velocity -= velocity * drag * deltaTime;
    } else {
        velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    GameObject::translate(velocity * deltaTime);

    for (auto child : children) {
        child->rotate(angularVelocity * deltaTime);
    }

    nextVelocity = velocity;
    nextPosition = get_position();
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

    // Multiply by 0.5 since every collision will be counted twice
    accelerate(first_acceleration * 0.5f);
    other.accelerate(second_acceleration * 0.5f);

    translate(first_acceleration * deltaTime * 0.5f);
    other.translate(second_acceleration * deltaTime * 0.5f);
}
