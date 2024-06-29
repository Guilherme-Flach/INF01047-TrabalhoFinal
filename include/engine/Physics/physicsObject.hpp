#ifndef PHYSICSOJBECT_HEADER
#define PHYSICSOBJECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"

class PhysicsObject : public GameObject {
  private:
    constexpr static const GLfloat speedLimit = 3.0f;

  protected:
    GLfloat drag;
    GLfloat mass;
    glm::vec4 speed;
    glm::vec4 acceleration;
    glm::vec3 angularSpeed;
    

  public:
    PhysicsObject(glm::vec4 position, GLfloat mass);
    void accelerate(glm::vec4 speed);
    void increase_acceleration(glm::vec4 acceleration);
    void increase_angularSpeed(glm::vec3 speed);
    void applyForce(glm::vec4 force);
    void update(GLfloat deltaTime);


    GLfloat get_drag() { return drag; };
    GLfloat get_mass() { return mass; };
    glm::vec4 get_speed() { return speed; };
    glm::vec4 get_acceleration() { return acceleration; };
    glm::vec3 get_angular_speed(){ return angularSpeed; };

    void set_drag(GLfloat drag) { this->drag = drag; }
    void set_mass(GLfloat mass) { this->mass = mass; }
    void set_speed(glm::vec4 speed) { this->speed = speed; }
    void set_acceleration(glm::vec4 acceleration) { this->acceleration = acceleration; }
    void set_angular_speed(glm::vec3 angularSpeed) { this->angularSpeed = angularSpeed; }

};

#endif // PHYSICSOBJECT_HEADER
