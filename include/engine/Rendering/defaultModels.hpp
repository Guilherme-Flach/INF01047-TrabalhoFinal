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
              GL_LINES) {
        this->line_width = 1.5f;
    }
};

class DotModel : public Model3D {
  public:
    DotModel()
        : Model3D::Model3D(std::vector<GLfloat>({0.0f, 0.0f, 0.0f, 1.0f}),
                           std::vector<GLuint>({0}),
                           std::vector<GLfloat>({0.0f, 0.0f, 0.0f, 0.5f}),
                           GL_POINTS) {
        this->line_width = 5.0f;
    }
};

class NoModel : public Model3D {
  public:
    NoModel()
        : Model3D::Model3D(std::vector<GLfloat>({}), std::vector<GLuint>({}),
                           std::vector<GLfloat>({}), GL_POINTS) {}
};

class DebugCubeModel : public Model3D {
  public:
    DebugCubeModel() : Model3D(std::vector<GLfloat>({
        -0.5f, 0.5f,  0.5f,  1.0f, // posição do vértice 0
        -0.5f, -0.5f, 0.5f,  1.0f, // posição do vértice 1
        0.5f,  -0.5f, 0.5f,  1.0f, // posição do vértice 2
        0.5f,  0.5f,  0.5f,  1.0f, // posição do vértice 3
        -0.5f, 0.5f,  -0.5f, 1.0f, // posição do vértice 4
        -0.5f, -0.5f, -0.5f, 1.0f, // posição do vértice 5
        0.5f,  -0.5f, -0.5f, 1.0f, // posição do vértice 6
        0.5f,  0.5f,  -0.5f, 1.0f, // posição do vértice 7
    }),
    std::vector<GLuint>({
        0, 1, 2, // triângulo 1
        7, 6, 5, // triângulo 2
        3, 2, 6, // triângulo 3
        4, 0, 3, // triângulo 4
        4, 5, 1, // triângulo 5
        1, 5, 6, // triângulo 6
        0, 2, 3, // triângulo 7
        7, 5, 4, // triângulo 8
        3, 6, 7, // triângulo 9
        4, 3, 7, // triângulo 10
        4, 1, 0, // triângulo 11
        1, 6, 2, // triângulo 12
    }),
    std::vector<GLfloat>({
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 0
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 1
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 2
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 3
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 4
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 5
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 6
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 7
    }),
    GL_TRIANGLES) {}
};

#endif // DEFAULTMODELS
