#ifndef RENDERER_HEADER
#define RENDERER_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include "model3D.hpp"
#include "engine/loader.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class Renderer {
  private:
    std::vector<Model3D> renderModels;
    std::vector<GameObject *> render;
    std::vector<GLuint> indices;
    GLuint programId;

    int renderType;

    void renderObject(GameObject &object);

  public:
    Renderer(GLuint programId);
    void renderObjects();
    void addObject(Model3D object);
    void destroy(GameObject &object);
};

#endif // RENDERER
