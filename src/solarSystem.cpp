#include "engine/Physics/solarSystem.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/collider.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/planet.hpp"
#include "engine/Physics/player.hpp"
#include "matrices.hpp"
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <fstream>

const float SolarSystem::physicsUpdateTime = 1.0f / 40.0f;

SolarSystem::SolarSystem()
    : planets(std::vector<Planet *>()), player(Player()),
      timeSinceLastFrame(0.0f) {
    LoadConfigFromFile("../../data/startingconfig.txt");
}

void SolarSystem::FixedUpdate(GLfloat deltaTime) {
    // Update at a constant rate:
    timeSinceLastFrame += deltaTime;
    if (timeSinceLastFrame >= physicsUpdateTime) {
        glm::vec4 pull;
        for (size_t i = 0; i < planets.size(); i++) {
            planets[i]->applyForce(calculateGravityPull(i, planets[i]));
        }

        player.get_ship().applyForce(
            calculateGravityPull(-1, &player.get_ship()));
        player.applyForce(calculateGravityPull(-1, &player));

        player.get_ship().physicsUpdate(physicsUpdateTime);
        player.physicsUpdate(physicsUpdateTime);

        for (size_t i = 0; i < planets.size(); i++) {
            planets[i]->physicsUpdate(physicsUpdateTime);
        }

        timeSinceLastFrame -= physicsUpdateTime;
    }
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

    Planet *p = new Planet(glm::vec4(x, y, z, 1.0f), radius, surface_gravity);
    p->set_velocity(glm::vec4(vel_x, vel_y, vel_z, 0.0f));

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
                auto collider = new SphereCollider(planet, {0.0, 0.0, 0.0, 1.0},
                                                   planet->get_radius());
                planet->addChild(*collider);
                Renderer::instance().addToRenderQueue(Renderer::GOURAUD,
                                                      planet);
                planets.push_back(planet);
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
