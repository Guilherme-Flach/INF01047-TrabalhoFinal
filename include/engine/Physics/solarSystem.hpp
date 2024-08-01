#ifndef SOLARSYSTEM_HEADER
#define SOLARSYSTEM_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/planet.hpp"
#include "engine/Physics/player.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class SolarSystem {
  private: 
    std::vector<Planet*> planets;
    Player player;

    Planet* ParsePlanetInfo(std::string line);
    void LoadConfigFromFile(const char *filename);
  public: 
    SolarSystem();

    constexpr static float physicsUpdateFreq = 1.0f / 80.0f;
    Player& get_player();

    void FixedUpdate(GLfloat deltaTime);
};

#endif // RENDEROBJECT
