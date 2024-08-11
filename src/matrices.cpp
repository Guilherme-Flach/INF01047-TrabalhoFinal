#include "matrices.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cstdio>

glm::mat4 Matrix(float m00, float m01, float m02, float m03, float m10,
                 float m11, float m12, float m13, float m20, float m21,
                 float m22, float m23, float m30, float m31, float m32,
                 float m33) {
    return glm::mat4(m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32,
                     m03, m13, m23, m33);
}

glm::mat4 Matrix_Identity() {
    return Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrix_Translate(float tx, float ty, float tz) {
    return Matrix(1.0f, 0.0f, 0.0f, tx, 0.0f, 1.0f, 0.0f, ty, 0.0f, 0.0f, 1.0f,
                  tz, 0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrix_Scale(float sx, float sy, float sz) {
    return Matrix(sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz,
                  0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrix_Rotate_X(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, c, -s, 0.0f, 0.0f, s, c, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrix_Rotate_Y(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return Matrix(c, 0.0f, s, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -s, 0.0f, c, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrix_Rotate_Z(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return Matrix(c, -s, 0.0f, 0.0f, s, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f);
}

float norm(glm::vec4 v) {
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return sqrt(vx * vx + vy * vy + vz * vz);
}

glm::vec4 normalize(glm::vec4 v) {
    float vector_size = norm(v);
    return glm::vec4(v.x / vector_size, v.y / vector_size, v.z / vector_size,
                     v.w);
}
glm::mat4 Matrix_Rotate(float angle, glm::vec4 axis) {
    float c = cos(angle);
    float s = sin(angle);

    glm::vec4 v = axis / norm(axis);

    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return Matrix(vx * vx * (1 - c) + c, vx * vy * (1 - c) - vz * s,
                  vx * vz * (1 - c) + vy * s, 0.0f, vx * vy * (1 - c) + vz * s,
                  vy * vy * (1 - c) + c, vy * vz * (1 - c) - vx * s, 0.0f,
                  vx * vz * (1 - c) - vy * s, vy * vz * (1 - c) + vx * s,
                  vz * vz * (1 - c) + c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 crossproduct(glm::vec4 u, glm::vec4 v) {
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;

    return glm::vec4(u2 * v3 - u3 * v2, u3 * v1 - u1 * v3, u1 * v2 - u2 * v1,
                     0.0f);
}

float dotproduct(glm::vec4 u, glm::vec4 v) {
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float u4 = u.w;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;
    float v4 = v.w;

    if (u4 != 0.0f || v4 != 0.0f) {
        PrintVector(u);
        PrintVector(v);
        fprintf(stderr, "ERROR: Produto escalar não definido para pontos.\n");
        std::exit(EXIT_FAILURE);
    }

    return u1 * v1 + u2 * v2 + u3 * v3;
}

glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector,
                             glm::vec4 up_vector) {
    glm::vec4 w = -1.0f * view_vector;
    glm::vec4 u = crossproduct(view_vector, up_vector);

    w = w / norm(w);
    u = u / norm(u);

    glm::vec4 v = crossproduct(w, u);

    glm::vec4 origin_o = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    float ux = u.x;
    float uy = u.y;
    float uz = u.z;
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;
    float wx = w.x;
    float wy = w.y;
    float wz = w.z;

    glm::vec4 displacement_camera = position_c - origin_o;

    return Matrix(ux, uy, uz, dotproduct(-1.0f * u, displacement_camera), vx,
                  vy, vz, dotproduct(-1.0f * v, displacement_camera), wx, wy,
                  wz, dotproduct(-1.0f * w, displacement_camera), 0.0f, 0.0f,
                  0.0f, 1.0f);
}

glm::mat4 Matrix_Orthographic(float l, float r, float b, float t, float n,
                              float f) {
    glm::mat4 M =
        Matrix(2.0f / (r - l), 0.0f, 0.0f, -(r + l) / (r - l), 0.0f,
               2.0f / (t - b), 0.0f, -(t + b) / (t - b), 0.0f, 0.0f,
               2.0f / (f - n), -(f + n) / (f - n), 0.0f, 0.0f, 0.0f, 1.0f);

    return M;
}

glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n,
                             float f) {
    float t = fabs(n) * tanf(field_of_view / 2.0f);
    float b = -t;
    float r = t * aspect;
    float l = -r;

    glm::mat4 P = Matrix(n, 0.0f, 0.0f, 0.0f, 0.0f, n, 0.0f, 0.0f, 0.0f, 0.0f,
                         n + f, -f * n, 0.0f, 0.0f, 1.0f, 0.0f);

    glm::mat4 M = Matrix_Orthographic(l, r, b, t, n, f);

    return -M * P;
}

glm::mat4 Matrix_ChangeCoordinates(glm::vec4 origin_from, glm::vec4 origin_to,
                                   glm::mat3x4 basis) {
    glm::vec4 displacement = origin_to - origin_from;
    return Matrix(basis[0].x, basis[0].y, basis[0].z,
                  dotproduct(-basis[0], displacement), basis[1].x, basis[1].y,
                  basis[1].z, dotproduct(-basis[1], displacement), basis[2].x,
                  basis[2].y, basis[2].z, dotproduct(-basis[2], displacement),
                  0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Matrix_ToParentCoordinates(glm::vec4 center, glm::mat3x4 basis) {
    return Matrix(basis[0].x, basis[1].x, basis[2].x, center.x, basis[0].y,
                  basis[1].y, basis[2].y, center.y, basis[0].z, basis[1].z,
                  basis[2].z, center.z, 0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat3x4 Matrix_ChangeBasis(glm::mat3x4 origin, glm::mat3x4 target) {
    glm::mat3 transform_matrix = glm::mat3(
        target[0].x, target[1].x, target[2].x, target[0].y, target[1].y,
        target[2].y, target[0].z, target[1].z, target[2].z);
    auto new_x = transform_matrix * origin[0];
    auto new_y = transform_matrix * origin[1];
    auto new_z = transform_matrix * origin[2];
    return {new_x.x, new_x.y, new_x.z, 0.0f,    new_y.x, new_y.y,
            new_y.z, 0.0f,    new_z.x, new_z.y, new_z.z, 0.0f};
}

glm::mat3x4 Matrix_PopBasis(glm::mat3x4 child, glm::mat3x4 parent) {
    glm::mat3 transform_matrix = glm::mat3(
        parent[0].x, parent[0].y, parent[0].z, parent[1].x, parent[1].y,
        parent[1].z, parent[2].x, parent[2].y, parent[2].z);
    auto new_x = transform_matrix * child[0];
    auto new_y = transform_matrix * child[1];
    auto new_z = transform_matrix * child[2];
    return {new_x.x, new_x.y, new_x.z, 0.0f,    new_y.x, new_y.y,
            new_y.z, 0.0f,    new_z.x, new_z.y, new_z.z, 0.0f};
}

glm::vec4 Matrix_Project(glm::vec4 source, glm::vec4 target) {
    return (dotproduct(source, target) / dotproduct(target, target)) * target;
}

glm::mat3x4 Matrix_Orthogonalize(glm::mat3x4 basis) {
    basis[1] -= Matrix_Project(basis[1], basis[0]);
    basis[2] = basis[2] - Matrix_Project(basis[2], basis[0]) -
               Matrix_Project(basis[2], basis[1]);
    basis[0] /= norm(basis[0]);
    basis[1] /= norm(basis[1]);
    basis[2] /= norm(basis[2]);
    return basis;
}

glm::mat4 Matrix_Invert(glm::mat4 m) {
    return Matrix(m[0][0], m[1][0], m[2][0], -m[0][3], m[0][1], m[1][1],
                  m[2][1], -m[1][3], m[0][2], m[2][1], m[2][2], -m[2][3],
                  m[3][0], m[3][1], m[3][2], m[3][3]);
}

void PrintMatrix(glm::mat4 M) {
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][0], M[1][0], M[2][0],
           M[3][0]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][1], M[1][1], M[2][1],
           M[3][1]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][2], M[1][2], M[2][2],
           M[3][2]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][3], M[1][3], M[2][3],
           M[3][3]);
}

void PrintVector(glm::vec4 v) {
    printf("\n");
    printf("[ %+0.2f ]\n", v[0]);
    printf("[ %+0.2f ]\n", v[1]);
    printf("[ %+0.2f ]\n", v[2]);
    printf("[ %+0.2f ]\n", v[3]);
}

void PrintMatrixVectorProduct(glm::mat4 M, glm::vec4 v) {
    auto r = M * v;
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n",
           M[0][0], M[1][0], M[2][0], M[3][0], v[0], r[0]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ] = [ %+0.2f ]\n",
           M[0][1], M[1][1], M[2][1], M[3][1], v[1], r[1]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n",
           M[0][2], M[1][2], M[2][2], M[3][2], v[2], r[2]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n",
           M[0][3], M[1][3], M[2][3], M[3][3], v[3], r[3]);
}

void PrintMatrixVectorProductDivW(glm::mat4 M, glm::vec4 v) {
    auto r = M * v;
    auto w = r[3];
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]          "
           "  [ %+0.2f ]\n",
           M[0][0], M[1][0], M[2][0], M[3][0], v[0], r[0], r[0] / w);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ] = [ %+0.2f ] =(div "
           "w)=> [ %+0.2f ]\n",
           M[0][1], M[1][1], M[2][1], M[3][1], v[1], r[1], r[1] / w);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]          "
           "  [ %+0.2f ]\n",
           M[0][2], M[1][2], M[2][2], M[3][2], v[2], r[2], r[2] / w);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]          "
           "  [ %+0.2f ]\n",
           M[0][3], M[1][3], M[2][3], M[3][3], v[3], r[3], r[3] / w);
}
