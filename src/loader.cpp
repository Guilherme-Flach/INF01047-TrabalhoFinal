#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <map>
#include <string>
#include "engine/loader.hpp"
#include "engine/camera/camera.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <sstream>
#include <fstream>

std::map<KeyAction, std::function<void(void)>> keymaps;

bool operator<(const KeyAction &first, const KeyAction &second) {
    return first.key > second.key ||
           (first.key <= second.key && first.action > second.action);
}

void addKeyMap(KeyAction data, std::function<void(void)> action) {
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

    glm::vec4 position = {0.0, 0.0, 0.0, 1.0};
    glm::vec4 target = {0.0, 0.0, 0.0, 1.0};
    LookAtCamera camera = LookAtCamera(position, target);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program_id);
        glm::mat4 projection;

        float nearplane = -0.1f;
        float farplane = -10.0f;

        float field_of_view = M_PI / 3.0f;
        projection =
            Matrix_Perspective(field_of_view, 1.0f, nearplane, farplane);
        glUniformMatrix4fv(view_uniform, 1, GL_FALSE,
                           glm::value_ptr(camera.get_view()));
        glUniformMatrix4fv(projection_uniform, 1, GL_FALSE,
                           glm::value_ptr(projection));
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
}
