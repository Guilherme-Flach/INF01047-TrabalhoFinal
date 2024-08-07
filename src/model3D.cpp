#include "engine/Rendering/model3D.hpp"
#include <GL/gl.h>
#include <cstdio>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>
#include <limits>
#include <algorithm>


#include <stdexcept>
#include <vector>
#include <tiny_obj_loader.h>

#include "matrices.hpp"

Model3D::Model3D() {};

Model3D::Model3D(const char* path) : vertexArrayId(0) {

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    ObjModel model(path);

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> textures;
    std::vector<GLuint> indices;
    //glm::vec3 bbox_min;
    //glm::vec3 bbox_max;

    ComputeNormals(&model);
    for (size_t shape = 0; shape < model.shapes.size(); ++shape)
    {
        size_t first_index = indices.size();
        size_t num_triangles = model.shapes[shape].mesh.num_face_vertices.size();

        const float minval = std::numeric_limits<float>::min();
        const float maxval = std::numeric_limits<float>::max();

        glm::vec3 bbox_min = glm::vec3(maxval,maxval,maxval);
        glm::vec3 bbox_max = glm::vec3(minval,minval,minval);

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model.shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model.shapes[shape].mesh.indices[3*triangle + vertex];

                indices.push_back(first_index + 3*triangle + vertex);

                const float vx = model.attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model.attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model.attrib.vertices[3*idx.vertex_index + 2];
                //printf("tri %d vert %d = (%.2f, %.2f, %.2f)\n", (int)triangle, (int)vertex, vx, vy, vz);
                vertices.push_back( vx ); // X
                vertices.push_back( vy ); // Y
                vertices.push_back( vz ); // Z
                vertices.push_back( 1.0f ); // W

                bbox_min.x = std::min(bbox_min.x, vx);
                bbox_min.y = std::min(bbox_min.y, vy);
                bbox_min.z = std::min(bbox_min.z, vz);
                bbox_max.x = std::max(bbox_max.x, vx);
                bbox_max.y = std::max(bbox_max.y, vy);
                bbox_max.z = std::max(bbox_max.z, vz);

                // Inspecionando o código da tinyobjloader, o aluno Bernardo
                // Sulzbach (2017/1) apontou que a maneira correta de testar se
                // existem normais e coordenadas de textura no ObjModel é
                // comparando se o índice retornado é -1. Fazemos isso abaixo.

                if ( idx.normal_index != -1 )
                {
                    const float nx = model.attrib.normals[3*idx.normal_index + 0];
                    const float ny = model.attrib.normals[3*idx.normal_index + 1];
                    const float nz = model.attrib.normals[3*idx.normal_index + 2];
                    normals.push_back( nx ); // X
                    normals.push_back( ny ); // Y
                    normals.push_back( nz ); // Z
                    normals.push_back( 0.0f ); // W
                }

                if ( idx.texcoord_index != -1 )
                {
                    const float u = model.attrib.texcoords[2*idx.texcoord_index + 0];
                    const float v = model.attrib.texcoords[2*idx.texcoord_index + 1];
                    textures.push_back( u );
                    textures.push_back( v );
                }
            }
        }

        this->renderType = GL_TRIANGLES;
    }

    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    vertices.size() * sizeof(float),
                    vertices.data());
    GLuint location = 0;            // "(location = 0)" em "shader_vertex.glsl"
    GLint number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0,
                          0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (!normals.empty()) {
        GLuint VBO_normal_coefficients_id;
        glGenBuffers(1, &VBO_normal_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER,
                     normals.size() * sizeof(float), NULL,
                     GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        normals.size() * sizeof(float),
                        normals.data());
        location = 1;             // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT,
                              GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (!textures.empty()) {
        GLuint VBO_texture_coefficients_id;
        glGenBuffers(1, &VBO_texture_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER,
                     textures.size() * sizeof(float), NULL,
                     GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        textures.size() * sizeof(float),
                        textures.data());
        location = 2;             // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT,
                              GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    numIndices = indices.size();

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint),
                    indices.data());
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
    //

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);

}

ObjModel::ObjModel(const char *filename, const char *basepath,
                   bool triangulate) {
    printf("Carregando objetos do arquivo \"%s\"...\n", filename);

    // Se basepath == NULL, então setamos basepath como o dirname do
    // filename, para que os arquivos MTL sejam corretamente carregados caso
    // estejam no mesmo diretório dos arquivos OBJ.
    std::string fullpath(filename);
    std::string dirname;
    if (basepath == NULL) {
        auto i = fullpath.find_last_of("/");
        if (i != std::string::npos) {
            dirname = fullpath.substr(0, i + 1);
            basepath = dirname.c_str();
        }
    }

    printf("basepath -> %s", basepath);

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                filename, basepath, triangulate);

    if (!err.empty())
        fprintf(stderr, "\n%s\n", err.c_str());

    if (!ret)
        throw std::runtime_error("Erro ao carregar modelo.");

    for (size_t shape = 0; shape < shapes.size(); ++shape) {
        if (shapes[shape].name.empty()) {
            fprintf(stderr,
                    "*********************************************\n"
                    "Erro: Objeto sem nome dentro do arquivo '%s'.\n"
                    "Veja "
                    "https://www.inf.ufrgs.br/~eslgastal/"
                    "fcg-faq-etc.html#Modelos-3D-no-formato-OBJ .\n"
                    "*********************************************\n",
                    filename);
            throw std::runtime_error("Objeto sem nome.");
        }
        printf("- Objeto '%s'\n", shapes[shape].name.c_str());
    }

    printf("OK.\n");
};

void Model3D::ComputeNormals(ObjModel *model) {
    if (!model->attrib.normals.empty())
        return;

    size_t num_vertices = model->attrib.vertices.size() / 3;

    std::vector<int> num_triangles_per_vertex(num_vertices, 0);
    std::vector<glm::vec4> vertex_normals(num_vertices,
                                          glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    for (size_t shape = 0; shape < model->shapes.size(); ++shape) {
        size_t num_triangles =
            model->shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle) {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            glm::vec4 vertices[3];
            for (size_t vertex = 0; vertex < 3; ++vertex) {
                tinyobj::index_t idx =
                    model->shapes[shape].mesh.indices[3 * triangle + vertex];
                const float vx =
                    model->attrib.vertices[3 * idx.vertex_index + 0];
                const float vy =
                    model->attrib.vertices[3 * idx.vertex_index + 1];
                const float vz =
                    model->attrib.vertices[3 * idx.vertex_index + 2];
                vertices[vertex] = glm::vec4(vx, vy, vz, 1.0);
            }

            const glm::vec4 a = vertices[0];
            const glm::vec4 b = vertices[1];
            const glm::vec4 c = vertices[2];

            // PREENCHA AQUI o cálculo da normal de um triângulo cujos vértices
            // estão nos pontos "a", "b", e "c", definidos no sentido
            // anti-horário.
            const glm::vec4 n = crossproduct(b-a, c-a);
            

            for (size_t vertex = 0; vertex < 3; ++vertex) {
                tinyobj::index_t idx =
                    model->shapes[shape].mesh.indices[3 * triangle + vertex];
                num_triangles_per_vertex[idx.vertex_index] += 1;
                vertex_normals[idx.vertex_index] += n;
                model->shapes[shape]
                    .mesh.indices[3 * triangle + vertex]
                    .normal_index = idx.vertex_index;
            }
        }
    }

    model->attrib.normals.resize(3 * num_vertices);

    for (size_t i = 0; i < vertex_normals.size(); ++i) {
        glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
        n /= norm(n);
        model->attrib.normals[3 * i + 0] = n.x;
        model->attrib.normals[3 * i + 1] = n.y;
        model->attrib.normals[3 * i + 2] = n.z;
    }

}