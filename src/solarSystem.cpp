#include "engine/Physics/solarSystem.hpp"
#include "GLFW/glfw3.h"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/collisions.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/planet.hpp"
#include "engine/Physics/player.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/matrix.hpp"
#include "matrices.hpp"
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <engine/Input/inputHandler.hpp>
#include "engine/loader.hpp"
#include "matrices.hpp"
#include <cmath>
#include <textrendering.hpp>
#include <iomanip>
#include <sstream>

const float SolarSystem::physicsUpdateTime = 1.0f / 50.0f;
CollisionsManager SolarSystem::collisionsManager;

// FONTE: Adapted from
// https://gamedev.stackexchange.com/questions/12360/how-do-you-determine-which-object-surface-the-users-pointing-at-with-lwjgl/12370#12370
glm::vec4 SolarSystem::getPlanetSpawnPosWithMouse() {
    // TODO: Maldade abaixo
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(Loader::get_window(), &windowWidth, &windowHeight);

    int mouseX = InputHandler::getMousePos().x;
    int mouseY = InputHandler::getMousePos().y;

    const float aspectRatio = (float)windowWidth / windowHeight;

    // get the mouse position in screenSpace coords
    double screenSpaceX =

        ((float)mouseX / (windowWidth / 2.0f) - 1.0f) * aspectRatio;
    double screenSpaceY = (1.0f - (float)mouseY / (windowHeight / 2.0f));

    double viewRatio = tan((player.get_panoramicCamera().get_fov()) / 2.00f);

    screenSpaceX = screenSpaceX * viewRatio;
    screenSpaceY = screenSpaceY * viewRatio;

    // Find the far and near camera spaces
    glm::vec4 cameraSpaceNear = glm::vec4(
        (float)(screenSpaceX * player.get_panoramicCamera().get_nearPlane()),
        (float)(screenSpaceY * player.get_panoramicCamera().get_nearPlane()),
        (float)(-player.get_panoramicCamera().get_nearPlane()), 1);
    glm::vec4 cameraSpaceFar = glm::vec4(
        (float)(screenSpaceX * player.get_panoramicCamera().get_farPlane()),
        (float)(screenSpaceY * player.get_panoramicCamera().get_farPlane()),
        (float)(-player.get_panoramicCamera().get_farPlane()), 1);

    // Unproject the 2D window into 3D to see where in 3D we're actually
    // clicking
    glm::mat4 tmpView = player.get_panoramicCamera().get_viewMatrix();
    glm::mat4 invView = glm::inverse(tmpView);

    glm::vec4 worldSpaceNear = invView * cameraSpaceNear;

    glm::vec4 worldSpaceFar = invView * cameraSpaceFar;

    // calculate the ray position and direction
    glm::vec4 rayPosition =
        glm::vec4(player.get_panoramicCamera().get_global_position());
    glm::vec4 rayDirection = glm::vec4(
        worldSpaceFar.x - worldSpaceNear.x, worldSpaceFar.y - worldSpaceNear.y,
        worldSpaceFar.z - worldSpaceNear.z, 0.0f);

    rayDirection = normalize(rayDirection);

    PhysicsObject temp = PhysicsObject(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f);

    RaycastCollider ray = RaycastCollider(&temp, rayPosition, rayDirection);

    PlaneCollider plane = PlaneCollider(
        &temp, ORIGIN, UP, glm::vec4(-20000.0f, 0.0f, -20000.0f, 1.0f),
        glm::vec4(20000.0f));

    CollisionData collision = plane.test_collision(ray);

    if (collision.isColliding) {
        return collision.collisionPoint;
    } else {
        return ORIGIN;
    }
}

glm::vec4 SolarSystem::getPlanetSpawnPosWithPlayer() {
    const auto direction =
        player.get_playerCamera().get_target()->get_global_position() -
        player.get_playerCamera().get_global_position();

    return player.get_playerCamera().get_target()->get_global_position() +
           (10.0f + 2 * spawnedPlanetSize) * direction;
}

SolarSystem::SolarSystem()
    : planets(std::vector<Planet *>()), player(Player()),
      timeSinceLastFrame(0.0f), spawnedPlanetSize(1.0f),
      spawnedPlanetVelocity(0.0f), spawnedPlanetVelocityIncreaseFlag(true) {
    LoadConfigFromFile("../../data/startingconfig.txt");

    collisionsManager.add_object(player.get_ship());

    InputHandler::addClickMapping(GLFW_MOUSE_BUTTON_1, [this](Action action) {
        if (action == GLFW_PRESS) {
            glm::vec4 spawnPosition;
            if (Loader::get_globalState(Loader::StateFlag::VIEW_TYPE) ==
                Loader::StateValue::VIEW_PANORAMIC) {
                // Spawn Planet on mouse position on screen
                spawnPosition = getPlanetSpawnPosWithMouse();
            } else if ((Loader::get_globalState(Loader::StateFlag::VIEW_TYPE) ==
                        Loader::StateValue::VIEW_SHIP)) {
                // Spawn planet where player is looking

                spawnPosition = getPlanetSpawnPosWithPlayer();
            }

            this->spawnPlanet(spawnPosition, spawnedPlanetSize,
                              pow(spawnedPlanetSize, 2) * 2,
                              spawnedPlanetVelocity);
        }
    });

    InputHandler::addScrollCallback([this](int xoffset, int yoffset) {
        spawnedPlanetSize += 0.05 * yoffset;
        if (spawnedPlanetSize < 0.05) {
            spawnedPlanetSize = 0.05;
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_LEFT_ALT, [this](Action action) {
        if (action == GLFW_PRESS) {
            spawnedPlanetVelocityIncreaseFlag = false;
        } else if (action == GLFW_RELEASE) {
            spawnedPlanetVelocityIncreaseFlag = true;
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_1, [this](Action action) {
        if (action == GLFW_PRESS) {
            float increase = 0.5f;
            if (!spawnedPlanetVelocityIncreaseFlag) {
                increase = -increase;
            }
            spawnedPlanetVelocity.x += increase;
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_2, [this](Action action) {
        if (action == GLFW_PRESS) {
            float increase = 0.5f;
            if (!spawnedPlanetVelocityIncreaseFlag) {
                increase = -increase;
            }
            spawnedPlanetVelocity.y += increase;
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_3, [this](Action action) {
        if (action == GLFW_PRESS) {
            float increase = 0.5f;
            if (!spawnedPlanetVelocityIncreaseFlag) {
                increase = -increase;
            }
            spawnedPlanetVelocity.z += increase;
        }
    });
}

void SolarSystem::FixedUpdate(GLfloat deltaTime) {
    // Update at a constant rate:
    timeSinceLastFrame += deltaTime;

    if (timeSinceLastFrame >= physicsUpdateTime) {
        for (size_t i = 0; i < planets.size(); i++) {
            planets[i]->applyForce(calculateGravityPull(i, planets[i]) *
                                   physicsUpdateTime);
        }

        player.get_ship().physicsUpdate(physicsUpdateTime);
        player.physicsUpdate(physicsUpdateTime);

        for (size_t i = 0; i < planets.size(); i++) {
            planets[i]->physicsUpdate(physicsUpdateTime);
        }

        collisionsManager.update_colliders();
        collisionsManager.handle_collisions(physicsUpdateTime);

        timeSinceLastFrame -= physicsUpdateTime;
    }

    for (auto planet : planets) {
        planet->update(deltaTime);
    }

    // Maldade:
    std::stringstream s;
    s << "Planet Size: " << std::fixed << std::setprecision(2)
      << spawnedPlanetSize;
    TextRendering_PrintString(Loader::get_window(), s.str(), -1.0f, -1.0f,
                              1.0f);

    TextRendering_PrintString(Loader::get_window(), "Planet Velocity", 0.69,
                              -0.825, 1.0f);

    TextRendering_PrintVector(Loader::get_window(), spawnedPlanetVelocity, 0.85,
                              -0.87);
}

Player &SolarSystem::get_player() { return player; }

glm::vec4 SolarSystem::calculateGravityPull(int index, PhysicsObject *obj) {
    const glm::vec4 objPosition = obj->get_global_position();

    glm::vec4 totalPull = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (size_t i = 0; i < planets.size(); i++) {
        if (i != index) { // ignore self
            const glm::vec4 direction =
                planets[i]->get_global_position() - objPosition;

            const float dist = norm(direction) + 0.001f;
            const glm::vec4 pull = PhysicsObject::G_CONSTANT *
                                   (direction / dist) * planets[i]->get_mass() *
                                   obj->get_mass() / (dist * dist);

            totalPull += pull;
        }
    }

    return totalPull;
}

Planet *SolarSystem::ParsePlanetInfo(std::string line) {
    const std::string positionLimiter = "p";
    const std::string radiusLimiter = "r";
    const std::string surfaceGravityLimiter = "g";
    const std::string initialVelocityLimier = "v";

    // clean up line
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

    int tokenLimiter;
    tokenLimiter = line.find(positionLimiter);
    float x = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(positionLimiter);
    float y = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(positionLimiter);
    float z = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(radiusLimiter);
    float radius = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(surfaceGravityLimiter);
    float surface_gravity = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(initialVelocityLimier);
    float vel_x = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(initialVelocityLimier);
    float vel_y = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(initialVelocityLimier);
    float vel_z = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    Planet *p = spawnPlanet(glm::vec4(x, y, z, 1.0f), radius, surface_gravity,
                            glm::vec4(vel_x, vel_y, vel_z, 0.0f));

    return p;
}

void SolarSystem::LoadConfigFromFile(const char *filename) {
    std::ifstream config_file(filename);
    std::string line;

    if (config_file.is_open()) {
        for (std::string line; std::getline(config_file, line);) {
            try {
                std::cout << "Read:" << "    " << line << std::endl;
                Planet *planet = ParsePlanetInfo(line);
            } catch (...) {
                std::cout << "Explodiu!" << std::endl;
                break;
            }
        }
        std::cout << "Read " << planets.size() << " planets." << std::endl;
        config_file.close();
    }
}

std::vector<Planet *> SolarSystem::get_planets() { return this->planets; }

SolarSystem::~SolarSystem() {
    std::vector<Planet *>::iterator planet;
    for (planet = planets.begin(); planet != planets.end(); planet++) {
        std::vector<GameObject *>::iterator collider;
        for (collider = (*planet)->get_children()->begin();
             collider != (*planet)->get_children()->end(); collider++) {
            if ((*collider)->get_type() == GameObjectType::COLLIDER) {
                delete *collider;
            }
        }
    }
}

Planet *SolarSystem::spawnPlanet(glm::vec4 position, GLfloat radius,
                                 GLfloat mass, glm::vec4 startingVelocity) {
    Planet *planet = new Planet(position, radius, mass);
    planet->set_velocity(startingVelocity);
    planets.push_back(planet);

    collisionsManager.add_object(*planet);

    return planet;
}
