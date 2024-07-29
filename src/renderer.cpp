#include "engine/Rendering/renderer.hpp"
#include "GLFW/glfw3.h"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "engine/loader.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

Renderer::Renderer(GLFWwindow *window)
    : window(window), programs(std::map<RenderMode, RenderProgram>()),
      renderModels(std::vector<Model3D>()),
      renderQueues(std::map<RenderMode, std::vector<GameObject *>>()) {
    createProgram(PHONG, "../../data/phong_shader_vertex.glsl",
                  "../../data/phong_shader_fragment.glsl");

    createProgram(GOURAUD, "../../data/gouraud_shader_vertex.glsl",
                  "../../data/gouraud_shader_fragment.glsl");
}

Renderer &Renderer::instance(GLFWwindow *window) {
    static Renderer *instanced;
    if (instanced == nullptr) {
        instanced = new Renderer(window);
    }
    return *instanced;
}

void Renderer::setDebugMode(bool debugMode) { this->debugMode = debugMode; }

void Renderer::renderRenderQueue(RenderMode renderMode, Camera *camera) {
    glUseProgram(programs[renderMode].program_id);
    glm::mat4 projection;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    projection =
        Matrix_Perspective(camera->get_fov(), width / height,
                           camera->get_nearPlane(), camera->get_farPlane());
    glUniformMatrix4fv(programs[renderMode].projection_uniform, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glUniformMatrix4fv(programs[renderMode].view_uniform, 1, GL_FALSE,
                       glm::value_ptr(camera->get_viewMatrix()));

    for (GameObject *gameObject : renderQueues[renderMode]) {
        if (gameObject != NULL &&
            (gameObject->get_isRenderable() || debugMode)) {
            glUniform1i(programs[renderMode].objectid_uniform, -1);
            glUniformMatrix4fv(programs[renderMode].model_uniform, 1, GL_FALSE,
                               glm::value_ptr(gameObject->get_model_matrix()));
            gameObject->get_model()->render();
        }
    }
}

void Renderer::createProgram(RenderMode renderMode,
                             const char *vertexShaderFile,
                             const char *fragmentShaderFile) {
    GLuint vertex_shader_id = loadShader_Vertex(vertexShaderFile);
    GLuint fragment_shader_id = loadShader_Fragment(fragmentShaderFile);

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

    // Os "Shader Objects" podem ser marcados para deleção após serem linkados
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    GLint model_uniform = glGetUniformLocation(program_id, "model");
    GLint view_uniform = glGetUniformLocation(program_id, "view");
    GLint projection_uniform = glGetUniformLocation(program_id, "projection");
    GLint object_id_uniform = glGetUniformLocation(program_id, "object_id");

    RenderProgram program =
        RenderProgram{program_id, model_uniform, view_uniform,
                      projection_uniform, object_id_uniform};

    programs[renderMode] = program;
}

Renderer::RenderProgram Renderer::get_programSpec(RenderMode renderMode) {
    return programs[renderMode];
}

void Renderer::loadShader(const char *filename, GLuint shader_id) {
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

GLuint Renderer::loadShader_Vertex(const char *filename) {

    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    loadShader(filename, vertex_shader_id);

    return vertex_shader_id;
}

GLuint Renderer::loadShader_Fragment(const char *filename) {

    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(filename, fragment_shader_id);

    return fragment_shader_id;
}

void Renderer::addToRenderQueue(RenderMode renderMode, GameObject *object) {
    renderQueues[renderMode].push_back(object);
}