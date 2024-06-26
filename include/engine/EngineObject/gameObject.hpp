#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/matrix_float4x4.hpp"

class GameObject {

  protected:
    glm::mat4 model_matrix;
    std::vector<GameObject *> children;
    glm::vec3 modelScaling = {1.0f, 1.0f, 1.0f};
    Model3D *model;
    GameObject *parent;
    bool isRenderable = false;
    void set_parent(GameObject &parent);

  public:
    GameObject(glm::vec4 position);
    std::vector<GameObject *> *get_children();
    GameObject *get_parent();
    glm::vec4 get_position();
    glm::vec4 get_global_position();
    glm::mat4 get_model_matrix();
    glm::mat3x4 get_global_basis();
    glm::mat3x4 get_basis();
    Model3D *get_model();
    glm::vec3 get_modelScaling();
    bool get_isRenderable();

    void set_model(Model3D &model);
    void set_modelScaling(glm::vec3 scaling);

    void set_position(glm::vec4 position);

    void addChild(GameObject &child);
    void translate(glm::vec4 offset);
    void rotate(glm::vec3 rotation);
};

#endif // GAMEOBJECT_HEADER
