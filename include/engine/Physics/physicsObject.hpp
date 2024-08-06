#ifndef PHYSICSOJBECT_HEADER
#define PHYSICSOJBECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"

class PhysicsObject : public GameObject {
  private:
    const static GLfloat speedLimit;
    const static GLfloat collisionAttenuation;

  protected:
    GLfloat drag;
    GLfloat mass;
    glm::vec4 velocity;
    glm::vec4 acceleration;
    glm::vec3 angularVelocity;
    glm::vec4 previousPosition;

    glm::vec4 nextPosition;
    glm::vec4 nextVelocity;

  public:
    const static float G_CONSTANT;

    PhysicsObject(glm::vec4 position, GLfloat mass);
    void accelerate(glm::vec4 velocity);
    void increase_acceleration(glm::vec4 acceleration);
    void increase_angularVelocity(glm::vec3 velocity);
    void applyForce(glm::vec4 force);
    void physicsUpdate(GLfloat deltaTime);

    GLfloat get_drag() { return drag; };
    GLfloat get_mass() { return mass; };
    glm::vec4 get_previous_position() { return previousPosition; };
    glm::vec4 get_velocity() { return velocity; };
    glm::vec4 get_acceleration() { return acceleration; };
    glm::vec3 get_angular_velocity() { return angularVelocity; };

    void translate(glm::vec4 direction);
    void set_drag(GLfloat drag) { this->drag = drag; }
    void set_mass(GLfloat mass) { this->mass = mass; }
    void set_velocity(glm::vec4 velocity) { this->nextVelocity = velocity; }
    void set_acceleration(glm::vec4 acceleration) {
        this->acceleration = acceleration;
    }
    void set_angular_velocity(glm::vec3 angularVelocity) {
        this->angularVelocity = angularVelocity;
    }

    void handle_collision(glm::vec4 collision_point, PhysicsObject &other,
                          GLfloat deltaTime);
};

#endif // PHYSICSOJBECT_HEADER
