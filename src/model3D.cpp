#include "engine/Rendering/model3D.hpp"
#include <GL/gl.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>
#include <vector>

Model3D::Model3D()
    : Model3D::Model3D(std::vector<GLfloat>(
                           {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
                       std::vector<GLuint>({0, 1, 2, 3, 4, 5}),
                       std::vector<GLfloat>(
                           {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
                       GL_LINES) {}

Model3D::Model3D(std::vector<GLfloat> vertices_, std::vector<GLuint> indices_,
                 std::vector<GLfloat> colors_, int renderType_)
    : vertices(vertices_), colors(colors_), indices(indices_), vertexArrayId(0),
      renderType(renderType_), line_width(4.0f) {

    // Generating VertexArray
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    // Vertex binding
    glGenBuffers(1, &verticesId);
    glBindBuffer(GL_ARRAY_BUFFER, verticesId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat),
                    &vertices[0]);

    GLuint location = 0;
    GLint number_of_dimensions = 4;

    // Gen pointer to model
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0,
                          0);
    glEnableVertexAttribArray(location);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Color binding
    glGenBuffers(1, &colorsId);
    glBindBuffer(GL_ARRAY_BUFFER, colorsId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat),
                    &colors[0]);
    location = 1;
    number_of_dimensions = 4;
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0,
                          0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Indices binding
    glGenBuffers(1, &indicesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint),
                    &indices[0]);

    glBindVertexArray(0);
}

GLuint Model3D::get_vertexArrayId() { return this->vertexArrayId; }

void Model3D::render() {
    glBindVertexArray(vertexArrayId);
    glLineWidth(line_width);
    glDrawElements(renderType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
