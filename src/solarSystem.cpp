#include "engine/Physics/solarSystem.hpp"
#include "engine/Physics/player.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

SolarSystem::SolarSystem() : planets(std::vector<Planet*>()), player(Player()) {
    LoadConfigFromFile("../../data/startingconfig.txt");
}


void SolarSystem::FixedUpdate(GLfloat deltaTime) {

}

Player& SolarSystem::get_player() {return player; }


Planet* SolarSystem::ParsePlanetInfo(std::string line) {
    const std::string tokenSeparator = ";";
    int tokenLimiter;
    tokenLimiter = line.find(tokenSeparator);
    float x = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(tokenSeparator);
    float y = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(tokenSeparator);
    float z = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(tokenSeparator);
    float radius = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(tokenSeparator);
    float surface_gravity = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(tokenSeparator);
    float vel_x = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(tokenSeparator);
    float vel_y = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    tokenLimiter = line.find(tokenSeparator);
    float vel_z = stof(line.substr(0, tokenLimiter));
    line = line.substr(tokenLimiter + 1); // erase until separator

    Planet* p = new Planet(glm::vec4(x, y, z, 1.0f), radius, surface_gravity);
    p->set_velocity(glm::vec4(vel_x, vel_y, vel_z, 0.0f));

    return p;
}

void SolarSystem::LoadConfigFromFile(const char *filename) {
    std::ifstream config_file(filename);
    std::string line;

    if (config_file.is_open()) {
        while (config_file) {
            try {
                std::getline(config_file, line);
                Planet* planet = ParsePlanetInfo(line);
                Renderer::instance().addToRenderQueue(Renderer::GOURAUD, planet);
                planets.push_back(planet);

            } catch (...) {
                std::cout << "Explodiu!" << std::endl;
            }
        }
        config_file.close();
    }
}