#ifndef RENDERER_HEADER
#define RENDERER_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include "model3D.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class Renderer {
  private:
    std::vector<Model3D> renderModels;
    GLuint programId;
    bool debugMode;

    GLint model_uniform;
    GLint view_uniform;
    GLint projection_uniform;
    GLint objectid_uniform;

    int renderType;

    Renderer(GLuint programId);

    void renderGameObject(GameObject *object);

  public:
    static Renderer &instance(GLuint programId);
    void setDebugMode(bool debugMode);
    void addModel(Model3D *object);
    void destroy(GameObject *object);
    void renderGameObjects(std::vector<GameObject *> &store);
};

#endif // RENDERER
