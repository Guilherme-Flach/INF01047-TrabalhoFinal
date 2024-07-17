#ifndef PLANET_HEADER
#define PLANET_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"

class Planet : public PhysicsObject {
  private:
    constexpr static const GLfloat speedLimit = 10.0f;

  protected:
    GLfloat gravity;
    

  public:
    Planet(glm::vec4 position, GLfloat mass, GLfloat gravity) : PhysicsObject(position, mass), gravity(gravity) {};

    GLfloat get_gravity() { return gravity; };

    void set_gravity(GLfloat gravity) { this->gravity = gravity; }

};

#endif // PLANET_HEADER
