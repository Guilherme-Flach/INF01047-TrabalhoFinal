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

  protected:
    GLfloat surfaceGravity;

  public:
    Planet(glm::vec4 position, GLfloat mass, GLfloat surfaceGravity) : PhysicsObject(position, mass), surfaceGravity(surfaceGravity) {};

    GLfloat get_surfaceGravity() { return surfaceGravity; };

    void set_surfaceGravity(GLfloat gravity) { this->surfaceGravity = gravity; }

};

#endif // PLANET_HEADER
