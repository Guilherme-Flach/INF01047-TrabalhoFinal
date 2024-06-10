#ifndef RENDEROBJECT_HEADER
#define RENDEROBJECT_HEADER
#include "engine/loader.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

#include <vector>

class RenderObject {
  protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    std::vector<GLuint> indices;

    GLuint vertexArrayId;
    GLuint colorsId;
    GLuint indicesId;
    GLuint verticesId;

    int renderType;

  public:
    GLuint get_vertexArrayId();
    RenderObject(std::vector<GLfloat> vertices_, std::vector<GLuint> indices_,
                 std::vector<GLfloat> colors_, int renderType_);
    void render();
};

#endif // RENDEROBJECT
