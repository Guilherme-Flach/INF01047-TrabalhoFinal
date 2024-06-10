#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <map>
#include <string>
#include "engine/loader.hpp"
#include "engine/camera/camera.hpp"
#include "engine/gameObject.hpp"
#include "engine/renderObject.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <sstream>
#include <fstream>

void TextRendering_ShowModelViewProjection(GLFWwindow *window,
                                           glm::mat4 projection, glm::mat4 view,
                                           glm::mat4 model, glm::vec4 p_model) {
    glm::vec4 p_world = model * p_model;
    glm::vec4 p_camera = view * p_world;
    glm::vec4 p_clip = projection * p_camera;
    glm::vec4 p_ndc = p_clip / p_clip.w;

    float pad = TextRendering_LineHeight(window);

    TextRendering_PrintString(
        window, " Model matrix             Model     In World Coords.", -1.0f,
        1.0f - pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, model, p_model, -1.0f,
                                           1.0f - 2 * pad, 1.0f);

    TextRendering_PrintString(window,
                              "                                        |  ",
                              -1.0f, 1.0f - 6 * pad, 1.0f);
    TextRendering_PrintString(window,
                              "                            .-----------'  ",
                              -1.0f, 1.0f - 7 * pad, 1.0f);
    TextRendering_PrintString(window,
                              "                            V              ",
                              -1.0f, 1.0f - 8 * pad, 1.0f);

    TextRendering_PrintString(
        window, " View matrix              World     In Camera Coords.", -1.0f,
        1.0f - 9 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, view, p_world, -1.0f,
                                           1.0f - 10 * pad, 1.0f);

    TextRendering_PrintString(window,
                              "                                        |  ",
                              -1.0f, 1.0f - 14 * pad, 1.0f);
    TextRendering_PrintString(window,
                              "                            .-----------'  ",
                              -1.0f, 1.0f - 15 * pad, 1.0f);
    TextRendering_PrintString(window,
                              "                            V              ",
                              -1.0f, 1.0f - 16 * pad, 1.0f);

    TextRendering_PrintString(
        window, " Projection matrix        Camera                    In NDC",
        -1.0f, 1.0f - 17 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProductDivW(window, projection, p_camera,
                                               -1.0f, 1.0f - 18 * pad, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glm::vec2 a = glm::vec2(-1, -1);
    glm::vec2 b = glm::vec2(+1, +1);
    glm::vec2 p = glm::vec2(0, 0);
    glm::vec2 q = glm::vec2(width, height);

    glm::mat4 viewport_mapping = Matrix(
        (q.x - p.x) / (b.x - a.x), 0.0f, 0.0f,
        (b.x * p.x - a.x * q.x) / (b.x - a.x), 0.0f, (q.y - p.y) / (b.y - a.y),
        0.0f, (b.y * p.y - a.y * q.y) / (b.y - a.y), 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    TextRendering_PrintString(
        window, "                                                       |  ",
        -1.0f, 1.0f - 22 * pad, 1.0f);
    TextRendering_PrintString(
        window, "                            .--------------------------'  ",
        -1.0f, 1.0f - 23 * pad, 1.0f);
    TextRendering_PrintString(
        window, "                            V                           ",
        -1.0f, 1.0f - 24 * pad, 1.0f);

    TextRendering_PrintString(
        window, " Viewport matrix           NDC      In Pixel Coords.", -1.0f,
        1.0f - 25 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProductMoreDigits(
        window, viewport_mapping, p_ndc, -1.0f, 1.0f - 26 * pad, 1.0f);
}

std::map<KeyAction, std::function<void(void)>> keymaps;

bool operator<(const KeyAction &first, const KeyAction &second) {
    return first.key > second.key ||
           (first.key <= second.key && first.action > second.action);
}

void addKeymap(KeyAction data, std::function<void(void)> action) {
    keymaps[data] = action;
}

void handleKeymaps(GLFWwindow *window, int key, int scan_code, int action,
                   int mod) {
    KeyAction key_action;
    key_action.key = key;
    key_action.action = action;
    auto iterator = keymaps.find(key_action);
    if (iterator == keymaps.end())
        return;
    keymaps[key_action]();
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

void Loader::start() {

    LoadShadersFromFiles();
    TextRendering_Init();
    GLint model_uniform = glGetUniformLocation(program_id, "model");
    GLint view_uniform = glGetUniformLocation(program_id, "view");
    GLint projection_uniform = glGetUniformLocation(program_id, "projection");
    GLint render_as_black_uniform =
        glGetUniformLocation(program_id, "render_as_black");

    glm::vec4 position = {2.0, 2.0, 2.0, 1.0};
    glm::vec4 target = {0.0, 0.0, 0.0, 1.0};
    LookAtCamera camera = LookAtCamera(position, target);
    glfwSetKeyCallback(window, handleKeymaps);

    // DEBUG
    std::vector<GLfloat> vertices = std::vector<GLfloat>({
        -0.5f, 0.5f,  0.5f,  1.0f, // posição do vértice 0
        -0.5f, -0.5f, 0.5f,  1.0f, // posição do vértice 1
        0.5f,  -0.5f, 0.5f,  1.0f, // posição do vértice 2
        0.5f,  0.5f,  0.5f,  1.0f, // posição do vértice 3
        -0.5f, 0.5f,  -0.5f, 1.0f, // posição do vértice 4
        -0.5f, -0.5f, -0.5f, 1.0f, // posição do vértice 5
        0.5f,  -0.5f, -0.5f, 1.0f, // posição do vértice 6
        0.5f,  0.5f,  -0.5f, 1.0f, // posição do vértice 7
    });

    std::vector<GLfloat> colors = std::vector<GLfloat>({
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 0
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 1
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 2
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 3
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 4
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 5
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 6
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 7
    });

    std::vector<GLuint> indices = std::vector<GLuint>({
        0, 1, 2, // triângulo 1
        7, 6, 5, // triângulo 2
        3, 2, 6, // triângulo 3
        4, 0, 3, // triângulo 4
        4, 5, 1, // triângulo 5
        1, 5, 6, // triângulo 6
        0, 2, 3, // triângulo 7
        7, 5, 4, // triângulo 8
        3, 6, 7, // triângulo 9
        4, 3, 7, // triângulo 10
        4, 1, 0, // triângulo 11
        1, 6, 2, // triângulo 12
    });

    RenderObject cuboRender =
        RenderObject(vertices, indices, colors, GL_TRIANGLES);
    GameObject cubo1 = GameObject({0.0f, 0.0f, 0.0f, 1.0f}, program_id);
    cubo1.set_model(&cuboRender);
    GameObject cubo2 = GameObject({1.0f, 0.0f, 0.0f, 1.0f}, program_id);
    cubo2.set_model(&cuboRender);
    GameObject cubo3 = GameObject({0.0f, 1.0f, 0.0f, 1.0f}, program_id);
    cubo3.set_model(&cuboRender);
    cubo3.set_modelScaling(glm::vec3{0.5f, 0.5f, 0.5f});
    cubo1.addChild(&cubo3);
    // DEBUG END

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program_id);
        glm::mat4 projection;

        float nearplane = -0.1f;
        float farplane = -10.0f;

        // DEBUG:

        float field_of_view = M_PI / 3.0f;
        projection =
            Matrix_Perspective(field_of_view, 1.0f, nearplane, farplane);
        glUniformMatrix4fv(projection_uniform, 1, GL_FALSE,
                           glm::value_ptr(projection));

        glUniformMatrix4fv(view_uniform, 1, GL_FALSE,
                           glm::value_ptr(camera.get_viewMatrix()));

        cubo1.render();
        cubo1.translate({0.01f, 0.0f, 0.0f, 0.0f});
        cubo2.render();
        cubo2.rotate({0.01f, 0.01f, 0.01f});

        glm::vec4 p_model(0.5f, 0.5f, 0.5f, 1.0f);

        // TextRendering_ShowModelViewProjection(window, projection,
        // camera.get_viewMatrix(), cubo.get_model_matrix(), p_model);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
}
