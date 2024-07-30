#ifndef MODEL3D_HEADER
#define MODEL3D_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>
#include "tiny_obj_loader.h"

#include <vector>

struct Texture {
  GLuint texture_id;
  GLuint sampler_id;
};

struct ObjModel {
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;

    // Este construtor lê o modelo de um arquivo utilizando a biblioteca tinyobjloader.
    // Veja: https://github.com/syoyo/tinyobjloader
    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true);
};

class Model3D {
  protected:
    std::string name;
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> textures;
    std::vector<GLuint> indices;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
    GLuint vertexArrayId;
    GLuint verticesId;
    GLuint normalsId;
    GLuint texturesId;
    GLuint indicesId;

    int renderType;

    GLfloat line_width;

  public:
    Model3D(const char* path);

    static void ComputeNormals(ObjModel* model);

    GLuint get_vertexArrayId();
    void render();
};

#endif // MODEL3D
