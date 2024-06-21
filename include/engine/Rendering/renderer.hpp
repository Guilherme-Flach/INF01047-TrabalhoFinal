#ifndef RENDERER_HEADER
#define RENDERER_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include "engine/gameObject.hpp"
#include "model3D.hpp"
#include "engine/loader.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class Renderer {
  protected:
    std::vector<Model3D> renderModels;
    std::vector<GameObject> render;
    std::vector<GLuint> indices;
    GLuint programId;

    int renderType;

  public:
    GLuint get_vertexArrayId();
    Renderer(GLuint programId);
    void renderObjects();
};

#endif // RENDERER
