#ifndef RENDEROBJECT_HEADER
#define RENDEROBJECT_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class PhysicsManager {
  protected:
    std::vector<GameObject *> vertices;
    std::vector<GLfloat> colors;
    std::vector<GLuint> indices;

    GLuint vertexArrayId;
    GLuint colorsId;
    GLuint indicesId;
    GLuint verticesId;

    int renderType;

    GLfloat line_width;

  public:

    Model3D(std::vector<GLfloat> vertices_, std::vector<GLuint> indices_,
            std::vector<GLfloat> colors_, int renderType_);
    GLuint get_vertexArrayId();
    void render();
};

#endif // RENDEROBJECT
