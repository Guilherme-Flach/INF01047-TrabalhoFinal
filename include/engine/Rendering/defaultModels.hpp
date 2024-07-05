#ifndef DEFAULTMODELS_HEADER
#define DEFAULTMODELS_HEADER
#include "engine/Rendering/model3D.hpp"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

class BaseAxesModel : public Model3D {
  public:
    BaseAxesModel()
        : Model3D::Model3D(
              std::vector<GLfloat>({0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
              std::vector<GLuint>({0, 1, 2, 3, 4, 5}),
              std::vector<GLfloat>({1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
              GL_LINES) {}
};

class WireCube : public Model3D {
  public:
    WireCube()
        : Model3D::Model3D(
              std::vector<GLfloat>(
                  {-0.5f, 0.5f,  0.5f,  1.0f, -0.5f, -0.5f, 0.5f,  1.0f,
                   0.5f,  -0.5f, 0.5f,  1.0f, 0.5f,  0.5f,  0.5f,  1.0f,
                   -0.5f, 0.5f,  -0.5f, 1.0f, -0.5f, -0.5f, -0.5f, 1.0f,
                   0.5f,  -0.5f, -0.5f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f}),
              std::vector<GLuint>({0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 7,
                                   7, 6, 6, 2, 6, 5, 5, 4, 5, 1, 7, 3}),
              std::vector<GLfloat>({0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                                    1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                                    1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f}),
              GL_LINES) { this->line_width = 1.5f; }
};

class DotModel : public Model3D {
  public:
    DotModel()
        : Model3D::Model3D(
              std::vector<GLfloat>(
                  {0.0f, 0.0f,  0.0f,  1.0f}),
              std::vector<GLuint>({0}),
              std::vector<GLfloat>({0.0f, 0.0f, 0.0f, 0.5f}),
              GL_POINTS) { this->line_width = 5.0f; }
};

class NoModel : public Model3D {
  public:
    NoModel()
        : Model3D::Model3D(
              std::vector<GLfloat>(
                  {}),
              std::vector<GLuint>({}),
              std::vector<GLfloat>({}),
              GL_POINTS) {}
};

#endif // DEFAULTMODELS
