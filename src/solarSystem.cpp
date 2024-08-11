#include "engine/Physics/solarSystem.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/collider.hpp"
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

const float SolarSystem::physicsUpdateTime = 1.0f / 50.0f;
CollisionsManager SolarSystem::collisionsManager;

// FONTE: Adapted from
// https://gamedev.stackexchange.com/questions/12360/how-do-you-determine-which-object-surface-the-users-pointing-at-with-lwjgl/12370#12370
glm::vec4 SolarSystem::getPlanetSpawnPos() {
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
        std::cout << "powershell" << std::endl;
        return ORIGIN;
    }
}

SolarSystem::SolarSystem()
    : planets(std::vector<Planet *>()), player(Player()),
      timeSinceLastFrame(0.0f), spawnedPlanetSize(1.0f) {
    LoadConfigFromFile("../../data/startingconfig.txt");

    collisionsManager.add_object(player.get_ship());

    InputHandler::addClickMapping(GLFW_MOUSE_BUTTON_1, [this](Action action) {
        const glm::vec4 direction = FRONT;
        if (action == GLFW_PRESS &&
            player.get_controlMode() == Player::PANORAMIC) {
            const auto direction =
                player.get_playerCamera().get_target()->get_global_position() -
                player.get_playerCamera().get_global_position();
            const glm::vec4 spawnPosition = getPlanetSpawnPos();
            this->spawnPlanet(spawnPosition, 0.5f, 1.0f);
        }
    });
}

void SolarSystem::update(GLfloat deltaTime) {
    this->spawnedPlanetSize += InputHandler::getScrollOffset().y;
}

void SolarSystem::FixedUpdate(GLfloat deltaTime) {
    // Update at a constant rate:
    timeSinceLastFrame += deltaTime;
    if (timeSinceLastFrame >= physicsUpdateTime) {
        glm::vec4 pull;
        for (size_t i = 0; i < planets.size(); i++) {
            planets[i]->applyForce(calculateGravityPull(i, planets[i]) *
                                   physicsUpdateTime);
        }

        // player.get_ship().applyForce(
        //     calculateGravityPull(-1, &player.get_ship()) *
        //     physicsUpdateTime);
        // player.applyForce(calculateGravityPull(-1, &player) *
        //                   physicsUpdateTime);

        player.get_ship().physicsUpdate(physicsUpdateTime);
        player.physicsUpdate(physicsUpdateTime);

        for (size_t i = 0; i < planets.size(); i++) {
            planets[i]->physicsUpdate(physicsUpdateTime);
        }

        collisionsManager.update_colliders();
        collisionsManager.handle_collisions(physicsUpdateTime);

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

    Planet *p = spawnPlanet(glm::vec4(x, y, z, 1.0f), radius, surface_gravity);
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
                                 GLfloat mass) {
    Planet *planet = new Planet(position, radius, mass);
    auto collider =
        new SphereCollider(planet, {0.0, 0.0, 0.0, 1.0}, planet->get_radius());
    planet->addChild(*collider);

    collisionsManager.add_object(*planet);
    planets.push_back(planet);
    Renderer::instance().addToRenderQueue(Renderer::GOURAUD, planet);

    return planet;
}