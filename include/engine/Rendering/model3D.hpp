#ifndef MODEL3D_HEADER
#define MODEL3D_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>
#include "tiny_obj_loader.h"

#include <vector>

struct Material {
    glm::vec3 Kd;
    glm::vec3 Ka;
    glm::vec3 Ks;
    GLfloat q;
};

struct Texture {
    GLuint texture_id;
    GLuint sampler_id;
};

struct ObjModel {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    ObjModel(const char *filename, const char *basepath = NULL,
             bool triangulate = true);
};

struct Model3D {
  public:
    GLuint vertexArrayId;
    int numIndices;
    int renderType;

    Model3D();
    Model3D(const char *path);
    static void ComputeNormals(ObjModel *model);
};

#endif // MODEL3D
