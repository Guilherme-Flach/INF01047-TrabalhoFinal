#include <iostream>
#include <functional>
#include <glad/glad.h>
#include <string>
#include "engine/loader.hpp"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Input/inputHandler.hpp"
#include "engine/Physics/solarSystem.hpp"
#include "engine/Rendering/renderer.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "matrices.hpp"
#include "GLFW/glfw3.h"

void TextRendering_Init();
void TextRendering_ShowFramesPerSecond(GLFWwindow *window);
void TextRendering_PrintString(GLFWwindow *window, const std::string &str,
                               float x, float y, float scale = 1.0f);

float Loader::delta_t = 0;
GLFWwindow* Loader::window = nullptr;
Camera* Loader::active_camera = nullptr;

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


void Loader::start(std::function<void(void)> act) {
    TextRendering_Init();

    glfwSetKeyCallback(window, InputHandler::handleKeyInput);
    glfwSetMouseButtonCallback(window, InputHandler::handleMouseClick);
    glfwSetCursorPosCallback(window, InputHandler::handleCursorPosChange);
    glfwSetCursorPos(window, 0, 0);
    //glfwFocusWindow(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Renderer& renderer = Renderer::instance();

    // renderer.setDebugMode(true);

    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
        });
    glfwSetWindowSize(window, 800, 800);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //SolarSystem solarSystem = SolarSystem();

    float prev_time = 0;
    while (!glfwWindowShouldClose(window)) {

        // Atualiza delta de tempo
        float current_time = (float)glfwGetTime();
        Loader::delta_t = current_time - prev_time;
        prev_time = current_time;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        act();       
        
        renderer.renderRenderQueue(Renderer::PHONG, active_camera, window);
        renderer.renderRenderQueue(Renderer::GOURAUD, active_camera, window);

        TextRendering_ShowFramesPerSecond(window);
        TextRendering_PrintString(window, ".", 0.0, 0.0, 2.0f);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
}
