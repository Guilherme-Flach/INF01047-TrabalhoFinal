#include <iostream>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "engine/loader.hpp"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Input/inputHandler.hpp"
#include "engine/Rendering/renderer.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "matrices.hpp"
#include <sstream>
#include <fstream>

void TextRendering_Init();
void TextRendering_ShowFramesPerSecond(GLFWwindow *window);

float Loader::delta_t = 0;

Loader::Loader(int width, int height, char title[]) {
    int success = glfwInit();
    if (!success) {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback([](int error, const char *description) {
        fprintf(stderr, "ERROR: GLFW: %s\n", description);
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSize(window, width, height);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *glversion = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion,
           glslversion);
    glEnable(GL_DEPTH_TEST);
}

void Loader::add_game_object(GameObject &object) {
    this->game_object_store.push_back(&object);
}

void Loader::add_camera(Camera &camera) {
    this->camera_store.push_back(&camera);
}

void Loader::set_active_camera(Camera *camera) { this->active_camera = camera; }

Planet Loader::ParsePlanetInfo(std::string line) {
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

    Planet p = Planet(glm::vec4(x, y, z, 1.0f), radius, surface_gravity);
    p.set_velocity(glm::vec4(vel_x, vel_y, vel_z, 0.0f));

    return p;
}

void Loader::LoadConfigFromFile(const char *filename) {
    std::ifstream config_file(filename);
    std::string line;

    if (config_file.is_open()) {
        while (config_file) {
            try {
                std::getline(config_file, line);
                Planet planet = ParsePlanetInfo(line);
                add_game_object(planet);
            } catch (...) {
                std::cout << "Explodiu!" << std::endl;
            }
        }
        config_file.close();
    }
}


void Loader::start(std::function<void(void)> act) {
    TextRendering_Init();

    glfwSetKeyCallback(window, InputHandler::handleKeyInput);
    glfwSetMouseButtonCallback(window, InputHandler::handleMouseClick);
    glfwSetCursorPosCallback(window, InputHandler::handleCursorPosChange);
    glfwSetCursorPos(window, 0, 0);
    //glfwFocusWindow(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Renderer& renderer = Renderer::instance(get_window());

    // renderer.setDebugMode(true);

    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
        });
    glfwSetWindowSize(window, 800, 800);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LoadConfigFromFile("../../data/startingconfig.txt");

    float prev_time = 0;
    while (!glfwWindowShouldClose(window)) {

        // Atualiza delta de tempo
        float current_time = (float)glfwGetTime();
        Loader::delta_t = current_time - prev_time;
        prev_time = current_time;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        act();       
        
        renderer.renderRenderQueue(Renderer::PHONG, active_camera);
        renderer.renderRenderQueue(Renderer::GOURAUD, active_camera);

        TextRendering_ShowFramesPerSecond(window);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
}
