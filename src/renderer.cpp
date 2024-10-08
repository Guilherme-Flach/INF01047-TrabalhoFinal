#include "engine/Rendering/renderer.hpp"
#include "GLFW/glfw3.h"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include "matrices.hpp"
#include "engine/Rendering/renderer.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <textrendering.hpp>
#include <engine/loader.hpp>

std::map<std::string, Texture> Renderer::textures =
    std::map<std::string, Texture>();
std::map<std::string, Model3D> Renderer::renderModels =
    std::map<std::string, Model3D>();
Renderer::Renderer()
    : programs(std::map<RenderMode, RenderProgram>()),
      renderQueues(std::map<RenderMode, std::vector<GameObject *>>()) {
    createProgram(PHONG, "../../data/shaders/phong_shader.vert",
                  "../../data/shaders/phong_shader.frag");

    createProgram(GOURAUD, "../../data/shaders/gouraud_shader.vert",
                  "../../data/shaders/gouraud_shader.frag");

    createProgram(SKYBOX, "../../data/shaders/skybox_shader.vert",
                  "../../data/shaders/skybox_shader.frag");

    // Load skybox
    skyboxTexture = loadCubemap("../../data/skybox/");

    skyboxModel = loadModel("skybox", "../../data/skybox/skybox.obj");
}

Renderer &Renderer::instance() {
    static Renderer *instanced;
    if (instanced == nullptr) {
        instanced = new Renderer();
    }
    return *instanced;
}

void Renderer::setDebugMode(bool debugMode) { this->debugMode = debugMode; }

void Renderer::renderRenderQueue(RenderMode renderMode, Camera *camera,
                                 GLFWwindow *window) {
    glUseProgram(programs[renderMode].program_id);
    glm::mat4 projection;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    projection =
        Matrix_Perspective(camera->get_fov(), (float)width / height,
                           camera->get_nearPlane(), camera->get_farPlane());
    glUniformMatrix4fv(programs[renderMode].projection_uniform, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glUniformMatrix4fv(programs[renderMode].view_uniform, 1, GL_FALSE,
                       glm::value_ptr(camera->get_viewMatrix()));

    for (GameObject *gameObject : renderQueues[renderMode]) {
        // Render the game object
        if (gameObject != NULL &&
            (gameObject->get_isRenderable() || debugMode)) {
            // Set texture
            if (gameObject->get_texture() != nullptr) {
                renderTexture(renderMode, *gameObject->get_texture());
            }
            glUniformMatrix4fv(programs[renderMode].model_uniform, 1, GL_FALSE,
                               glm::value_ptr(gameObject->get_model_matrix()));
            renderModel(*gameObject->get_model());
        }
    }
}

void Renderer::renderModel(Model3D model) {
    if (model.numIndices == 0) {
        return;
    }
    glBindVertexArray(model.vertexArrayId);
    glDrawElements(model.renderType, model.numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::renderTexture(RenderMode renderMode, Texture texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindSampler(0, texture.sampler_id);
    glBindTexture(GL_TEXTURE_2D, texture.texture_id);

    glUniform1i(programs[renderMode].texture0, 0);
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
    GLint texture0_uniform = glGetUniformLocation(program_id, "Texture0");

    RenderProgram program =
        RenderProgram{program_id, model_uniform, view_uniform,
                      projection_uniform, texture0_uniform};

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

void Renderer::renderSkybox(GLFWwindow *window, Camera *camera) {
    // FONTE: https://learnopengl.com/Advanced-OpenGL/Cubemaps (também usado
    // para o vertex e fragment shader de skybox)
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    glUseProgram(programs[SKYBOX].program_id);
    glm::mat4 projection;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    projection =
        Matrix_Perspective(camera->get_fov(), (float)width / height,
                           camera->get_nearPlane(), camera->get_farPlane());
    glUniformMatrix4fv(programs[SKYBOX].projection_uniform, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glUniformMatrix4fv(
        programs[SKYBOX].view_uniform, 1, GL_FALSE,
        glm::value_ptr(glm::mat4(glm::mat3(camera->get_viewMatrix()))));
    glBindVertexArray(skyboxModel->vertexArrayId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawElements(skyboxModel->renderType, skyboxModel->numIndices,
                   GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

void Renderer::renderScene(Camera *camera, GLFWwindow *window) {

    // Render Skybox
    renderSkybox(window, camera);

    renderRenderQueue(GOURAUD, camera, window);
    renderRenderQueue(PHONG, camera, window);

    if (Loader::get_globalState(Loader::StateFlag::VIEW_TYPE) ==
        Loader::StateValue::VIEW_SHIP) {
        TextRendering_PrintString(window, ".", 0.0, 0.0, 2.0f);
    }
}

Model3D *Renderer::loadModel(std::string name, const char *filename) {
    auto modelIterator = renderModels.find(name);

    if (modelIterator != renderModels.end()) {
        return &modelIterator->second;
    }

    renderModels[name] = Model3D(filename);
    return &renderModels[name];
}

Texture *Renderer::loadTexture(std::string name, const char *filename) {
    auto textureIterator = textures.find(name);

    if (textureIterator != textures.end()) {
        return &textureIterator->second;
    }

    printf("Carregando imagem \"%s\"... ", filename);

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);

    if (data == NULL) {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    textures[name] = Texture{texture_id, sampler_id};

    return &textures[name];
}

GLuint Renderer::loadCubemap(std::string path) {
    std::vector<std::string> faces = {"right.jpg",  "left.jpg",  "top.jpg",
                                      "bottom.jpg", "front.jpg", "back.jpg"};

    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load((path + faces[i]).c_str(), &width,
                                        &height, &nrChannels, 3);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                         height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap tex failed to load at path: "
                      << path + faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    textures["skybox"] = Texture{texture_id, sampler_id};

    return texture_id;
}
