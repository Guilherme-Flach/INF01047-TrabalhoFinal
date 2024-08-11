#ifndef _MATRICES_H
#define _MATRICES_H

#include "glm/ext/vector_float4.hpp"
#include <cstdio>
#include <cstdlib>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

static glm::vec4 ORIGIN = {0.0f, 0.0f, 0.0f, 1.0f};

static glm::vec4 FRONT = {0.0f, 0.0f, +1.0f, 1.0f};
static glm::vec4 BACK = {0.0f, 0.0f, -1.0f, 1.0f};

static glm::vec4 UP = {0.0f, +1.0f, 0.0f, 0.0f};
static glm::vec4 DOWN = {0.0f, -1.0f, 0.0f, 1.0f};

static glm::vec4 LEFT = {+1.0f, 0.0f, 0.0f, 1.0f};
static glm::vec4 RIGHT = {-1.0f, 0.0f, 0.0f, 1.0f};

static glm::vec4 NONE = {0.0f, 0.0f, 0.0f, 0.0f};

glm::mat4 Matrix(float m00, float m01, float m02, float m03, float m10,
                 float m11, float m12, float m13, float m20, float m21,
                 float m22, float m23, float m30, float m31, float m32,
                 float m33);

glm::mat4 Matrix_Identity();

glm::mat4 Matrix_Translate(float tx, float ty, float tz);

glm::mat4 Matrix_Scale(float sx, float sy, float sz);

glm::mat4 Matrix_Rotate_X(float angle);

glm::mat4 Matrix_Rotate_Y(float angle);

glm::mat4 Matrix_Rotate_Z(float angle);

float norm(glm::vec4 v);

glm::mat4 Matrix_Rotate(float angle, glm::vec4 axis);

glm::vec4 crossproduct(glm::vec4 u, glm::vec4 v);

float dotproduct(glm::vec4 u, glm::vec4 v);

glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector,
                             glm::vec4 up_vector);

glm::mat4 Matrix_Orthographic(float l, float r, float b, float t, float n,
                              float f);

glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n,
                             float f);

/* Calculates the change of basis matrix between two systems of coordinates.
 * Be mindful that this requires both systems to be orthonormal.
 */
glm::mat4 Matrix_ChangeCoordinates(glm::vec4 origin_from, glm::vec4 origin_to,
                                   glm::mat3x4 basis);

glm::mat3x4 Matrix_ChangeBasis(glm::mat3x4 origin, glm::mat3x4 target);

glm::mat3x4 Matrix_PopBasis(glm::mat3x4 child, glm::mat3x4 parent);

/* Calculates restores from a local coordinates system to its parent system.
 * Be mindful that this requires both systems to be orthonormal.
 */
glm::mat4 Matrix_ToParentCoordinates(glm::vec4 center, glm::mat3x4 basis);
glm::vec4 Matrix_Project(glm::vec4 source, glm::vec4 target);
glm::mat3x4 Matrix_Orthogonalize(glm::mat3x4 basis);
glm::mat4 Matrix_Invert(glm::mat4 m);

void PrintMatrix(glm::mat4 M);

void PrintVector(glm::vec4 v);

void PrintMatrixVectorProduct(glm::mat4 M, glm::vec4 v);

void PrintMatrixVectorProductDivW(glm::mat4 M, glm::vec4 v);

#endif
