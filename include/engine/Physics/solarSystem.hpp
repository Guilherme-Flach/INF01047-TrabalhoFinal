#ifndef SOLARSYSTEM_HEADER
#define SOLARSYSTEM_HEADER
#include "engine/Physics/planet.hpp"
#include "engine/Physics/player.hpp"
#include <cstddef>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class SolarSystem {
  private:
    std::vector<Planet *> planets;
    Player player;
    float timeSinceLastFrame;
    static CollisionsManager collisionsManager;

    Planet *ParsePlanetInfo(std::string line);
    void LoadConfigFromFile(const char *filename);
    glm::vec4 calculateGravityPull(int index, PhysicsObject *obj);
    Planet *spawnPlanet(glm::vec4 position, GLfloat radius, GLfloat mass);

  public:
    ~SolarSystem();
    SolarSystem();

    const static float physicsUpdateTime;
    Player &get_player();

    void FixedUpdate(GLfloat deltaTime);

    std::vector<Planet *> get_planets();
};

#endif // RENDEROBJECT
