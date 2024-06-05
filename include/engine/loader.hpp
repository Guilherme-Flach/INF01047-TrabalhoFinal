#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utils.h>
#include <matrices.h>
#include "camera/camera.hpp"
#include "glm/ext/vector_float4.hpp"

struct KeyAction {
    int key;
    int action;
};

inline bool operator<(const KeyAction &first, const KeyAction &second);
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

    Loader(int width, int height, char title[]);

  private:
    GLFWwindow *window;

    void LoadShader(const char *filename, GLuint shader_id);
    GLuint LoadShader_Vertex(const char *filename);
    GLuint LoadShader_Fragment(const char *filename);
    void LoadShadersFromFiles();

  public:
    GLFWwindow *get_window() { return window; }

    void start();
};
