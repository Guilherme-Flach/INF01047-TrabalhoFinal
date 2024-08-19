#ifndef PLANET_HEADER
#define PLANET_HEADER
#include <cstdlib>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/collisions.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"
#include "engine/Rendering/renderer.hpp"
#include "matrices.hpp"

const int NUMTEXTURES = 16;

class Planet : public PhysicsObject {
  private:
  protected:
    GLfloat surfaceGravity;
    GLfloat radius;
    GLfloat rotationSpeed;

    SphereCollider planetCollider;

    GameObject modelContainer;

  public:
    Planet(glm::vec4 position, GLfloat radius, GLfloat mass)
        : PhysicsObject(position, mass), radius(radius), surfaceGravity(mass),
          rotationSpeed((float)rand() / (float)RAND_MAX),
          modelContainer(GameObject(STANDARD, ORIGIN)),
          planetCollider(SphereCollider(this, {0.0, 0.0, 0.0, 1.0}, radius)) {
        static int planetCount = 0;

        const int textureIndex = planetCount % NUMTEXTURES;
        const auto texturePath =
            "../../data/planets/" + std::to_string(textureIndex) + ".png";
        modelContainer.set_modelScaling(radius);
        modelContainer.set_texture(Renderer::instance().loadTexture(
            std::to_string(textureIndex), texturePath.c_str()));
        modelContainer.set_model(Renderer::instance().loadModel(
            "planet", "../../data/planets/sphere.obj"));

        modelContainer.rotate(glm::vec3(rand() * M_PI * 2.0f / RAND_MAX,
                                        rand() * M_PI * 2.0f / RAND_MAX,
                                        rand() * M_PI * 2.0f / RAND_MAX));

        set_onUpdate([this](GLfloat deltaTime) -> void {
            modelContainer.rotate(rotationSpeed * deltaTime,
                                  glm::mat4(glm::mat3(get_model_matrix())) *
                                      glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
        });

        this->addChild(modelContainer);
        this->addChild(planetCollider);

        Renderer::instance().addToRenderQueue(Renderer::GOURAUD,
                                              &modelContainer);

        planetCount++;
    };

    GLfloat get_surfaceGravity() { return surfaceGravity; };
    GLfloat get_radius() { return radius; };

    void set_surfaceGravity(GLfloat gravity) { this->surfaceGravity = gravity; }
};
#endif // PLANET_HEADER
