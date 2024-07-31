#ifndef RENDERER_HEADER
#define RENDERER_HEADER
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "model3D.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class Renderer {
  public:
    enum RenderMode {
      PHONG,
      GOURAUD
    };
    
    struct RenderProgram {
      GLuint program_id;
      GLint model_uniform;
      GLint view_uniform;
      GLint projection_uniform;
      Material material_uniform;
      GLint texture0;
    };
  private:
    Renderer(GLFWwindow *window);
    GLFWwindow *window;

    static std::vector<Model3D> renderModels;
    static std::map<const char*, Texture> textures;
    std::map<RenderMode, std::vector<GameObject *>>  renderQueues;
    std::map<RenderMode, RenderProgram> programs;

    bool debugMode = false;

    void createProgram(RenderMode renderMode, const char *vertexShaderFile, const char *fragmentShaderFile);

    void loadShader(const char *filename, GLuint shader_id);
    GLuint loadShader_Vertex(const char *filename);
    GLuint loadShader_Fragment(const char *filename);
    
  public:
    void renderRenderQueue(RenderMode renderMode, Camera *camera);
    void addToRenderQueue(RenderMode renderMode, GameObject *object);
    static Renderer &instance(GLFWwindow *window);
    
    static void addModel(Model3D *object);
    static Texture& loadTexture(const char* name, const char* filename);
    void setDebugMode(bool debugMode);

    RenderProgram get_programSpec(RenderMode renderMode);
};

#endif // RENDERER
