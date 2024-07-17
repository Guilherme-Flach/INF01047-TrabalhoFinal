#ifndef RENDEROBJECT_HEADER
#define RENDEROBJECT_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/planet.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class PhysicsManager {
  private: 
    std::vector<Planet*> planets = std::vector<Planet*>();
  public: 
    PhysicsManager();

    constexpr static float physicsUpdateFreq = 1.0f / 80;

    void FixedUpdate(GLfloat deltaTime);

    void addPlanet(Planet &planet);
};

#endif // RENDEROBJECT
