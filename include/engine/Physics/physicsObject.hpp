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
    constexpr static const GLfloat speedLimit = 10.0f;

  protected:
    GLfloat mass;
    glm::vec4 speed;
    

  public:
    PhysicsObject(glm::vec4 position, GLfloat mass);
    void accelerate(glm::vec4 speed);
    void applyForce(glm::vec4 force);
    void update(GLfloat deltaTime);

    double get_mass() { return mass; };
    glm::vec4 get_speed() { return speed; };

    void set_mass(double mass) { this->mass = mass; }
    void set_speed(glm::vec4 speed) { this->speed = speed; }

};

#endif // PHYSICSOBJECT_HEADER
