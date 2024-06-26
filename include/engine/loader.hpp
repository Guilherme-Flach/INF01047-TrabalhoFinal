#ifndef LOADER_HEADER
#define LOADER_HEADER

#include <functional>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Input/inputHandler.hpp"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include <string>
#include <utils.h>
#include <matrices.hpp>
#include <vector>
#include "glm/ext/vector_float4.hpp"

void TextRendering_Init();
float TextRendering_LineHeight(GLFWwindow *window);
float TextRendering_CharWidth(GLFWwindow *window);
void TextRendering_PrintString(GLFWwindow *window, const std::string &str,
                               float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrix(GLFWwindow *window, glm::mat4 M, float x,
                               float y, float scale = 1.0f);
void TextRendering_PrintVector(GLFWwindow *window, glm::vec4 v, float x,
                               float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProduct(GLFWwindow *window, glm::mat4 M,
                                            glm::vec4 v, float x, float y,
                                            float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductMoreDigits(GLFWwindow *window,
                                                      glm::mat4 M, glm::vec4 v,
                                                      float x, float y,
                                                      float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductDivW(GLFWwindow *window, glm::mat4 M,
                                                glm::vec4 v, float x, float y,
                                                float scale = 1.0f);

void TextRendering_ShowModelViewProjection(GLFWwindow *window,
                                           glm::mat4 projection, glm::mat4 view,
                                           glm::mat4 model, glm::vec4 p_model);
void TextRendering_ShowEulerAngles(GLFWwindow *window);
void TextRendering_ShowProjection(GLFWwindow *window);
void TextRendering_ShowFramesPerSecond(GLFWwindow *window);
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);

class Loader {

  public:
    int program_id = 0;
    InputHandler* inputHandler;

    Loader(int width, int height, char title[]);

    void add_game_object(GameObject &object);
    void add_camera(Camera &camera);
    void set_active_camera(Camera *camera);

  private:
    static float delta_t;
    GLFWwindow *window;
    std::vector<GameObject *> game_object_store;
    std::vector<Camera *> camera_store;
    Camera *active_camera;

    void LoadShader(const char *filename, GLuint shader_id);
    GLuint LoadShader_Vertex(const char *filename);
    GLuint LoadShader_Fragment(const char *filename);
    void LoadShadersFromFiles();

  public:
    GLFWwindow *get_window() { return window; }

    static float get_delta_t() { return Loader::delta_t; }

    void start(std::function<void(void)> act);
};
#endif // LOADER_HEADER
