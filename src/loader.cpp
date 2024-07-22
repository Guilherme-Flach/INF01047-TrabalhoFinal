#include <iostream>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
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

float screen_ratio;

float Loader::delta_t = 0;

void printText(GLFWwindow *window, std::string text, float x, float y,
               float scale) {
    float pad = TextRendering_LineHeight(window);

    TextRendering_PrintString(window, text, x, y - pad, scale);
}

GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {

    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);

    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    if (linked_ok == GL_FALSE) {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        GLchar *log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        delete[] log;

        fprintf(stderr, "%s", output.c_str());
    }
    return program_id;
}

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

void Loader::LoadShader(const char *filename, GLuint shader_id) {
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch (std::exception &e) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar *shader_string = str.c_str();
    const GLint shader_string_length = static_cast<GLint>(str.length());

    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    glCompileShader(shader_id);

    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    GLchar *log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    if (log_length != 0) {
        std::string output;

        if (!compiled_ok) {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        } else {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    delete[] log;
}

GLuint Loader::LoadShader_Vertex(const char *filename) {

    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    LoadShader(filename, vertex_shader_id);

    return vertex_shader_id;
}

GLuint Loader::LoadShader_Fragment(const char *filename) {

    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    LoadShader(filename, fragment_shader_id);

    return fragment_shader_id;
}

void Loader::LoadShadersFromFiles() {

    GLuint vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id =
        LoadShader_Fragment("../../src/shader_fragment.glsl");

    if (program_id != 0)
        glDeleteProgram(program_id);

    program_id = CreateGpuProgram(vertex_shader_id, fragment_shader_id);
}

void Loader::add_game_object(GameObject &object) {
    this->game_object_store.push_back(&object);
}

void Loader::add_camera(Camera &camera) {
    this->camera_store.push_back(&camera);
}

void Loader::set_active_camera(Camera *camera) { this->active_camera = camera; }

void Loader::start(std::function<void(void)> act) {
    LoadShadersFromFiles();
    TextRendering_Init();
    GLint view_uniform = glGetUniformLocation(program_id, "view");
    GLint projection_uniform = glGetUniformLocation(program_id, "projection");
    GLint objectid_uniform = glGetUniformLocation(program_id, "object_id");

    glfwSetKeyCallback(window, InputHandler::handleKeyInput);
    glfwSetMouseButtonCallback(window, InputHandler::handleMouseClick);
    glfwSetCursorPosCallback(window, InputHandler::handleCursorPosChange);
    glfwSetCursorPos(window, 0, 0);
    glfwFocusWindow(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Renderer renderer = Renderer::instance(program_id);
    // renderer.setDebugMode(true);

    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
            screen_ratio = (float)width / height;
        });
    glfwSetWindowSize(window, 800, 800);
    screen_ratio = 1;

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float prev_time = 0;
    while (!glfwWindowShouldClose(window)) {

        // Atualiza delta de tempo
        float current_time = (float)glfwGetTime();
        Loader::delta_t = current_time - prev_time;
        prev_time = current_time;

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program_id);
        glm::mat4 projection;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        projection =
            Matrix_Perspective(this->active_camera->get_fov(), screen_ratio,
                               this->active_camera->get_nearPlane(),
                               this->active_camera->get_farPlane());
        glUniformMatrix4fv(projection_uniform, 1, GL_FALSE,
                           glm::value_ptr(projection));

        glUniformMatrix4fv(
            view_uniform, 1, GL_FALSE,
            glm::value_ptr(this->active_camera->get_viewMatrix()));
        act();

        renderer.renderGameObjects(this->game_object_store);

        TextRendering_ShowFramesPerSecond(window);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
}
