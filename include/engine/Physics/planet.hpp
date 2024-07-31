#ifndef PLANET_HEADER
#define PLANET_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engine/Physics/physicsObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"
#include "engine/Rendering/renderer.hpp"

class Planet : public PhysicsObject {
  private:
  protected:
    GLfloat surfaceGravity;

  public:
    Planet(glm::vec4 position, GLfloat radius, GLfloat surfaceGravity)
        : PhysicsObject(position, (surfaceGravity * radius * radius / PhysicsObject::G_CONSTANT)), surfaceGravity(surfaceGravity){
          set_modelScaling(radius);
          texture = Renderer::instance().loadTexture("ag", "../../data/planets/ag.jpg");
          model = Renderer::instance().loadModel("planet", "../../data/planets/sphere.obj");
        };

    GLfloat get_surfaceGravity() { return surfaceGravity; };

    void set_surfaceGravity(GLfloat gravity) { this->surfaceGravity = gravity; }
};

#endif // PLANET_HEADER
