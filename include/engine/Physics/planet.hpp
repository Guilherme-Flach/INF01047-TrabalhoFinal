#ifndef PLANET_HEADER
#define PLANET_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include "engine/Physics/physicsObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"
#include "engine/Rendering/renderer.hpp"

const int NUMTEXTURES = 16;

class Planet : public PhysicsObject {
  private:
  protected:
    GLfloat surfaceGravity;
    GLfloat radius;

  public:
    Planet(glm::vec4 position, GLfloat radius, GLfloat mass)
        : PhysicsObject(position, mass), radius(radius), surfaceGravity(mass) {
        static int planetCount = 0;

        const int textureIndex = planetCount % NUMTEXTURES;
        const auto texturePath =
            "../../data/planets/" + std::to_string(textureIndex) + ".png";
        set_modelScaling(radius);
        texture = Renderer::instance().loadTexture(std::to_string(textureIndex),
                                                   texturePath.c_str());
        model = Renderer::instance().loadModel("planet",
                                               "../../data/planets/sphere.obj");
        planetCount++;
    };

    GLfloat get_surfaceGravity() { return surfaceGravity; };
    GLfloat get_radius() { return radius; };

    void set_surfaceGravity(GLfloat gravity) { this->surfaceGravity = gravity; }
};
#endif // PLANET_HEADER
