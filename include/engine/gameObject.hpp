#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engine/loader.hpp"
#include "glm/ext/vector_float3.hpp"
#include "renderObject.hpp"

#include <vector>

class GameObject {

  protected:
    GLuint program_id;
    glm::vec4 position;
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 modelScaling = {1.0f, 1.0f, 1.0f};
    std::vector<GameObject> children;
    GameObject *parent = nullptr;
    RenderObject *model;

  public:
    GameObject(glm::vec4 position, GLuint program_id);
    std::vector<GameObject> get_children();
    RenderObject *get_model();
    glm::vec3 get_modelScaling();
    GameObject *get_parent();
    glm::vec3 get_rotation();
    glm::vec4 get_position();
    glm::mat4 get_model_matrix();

    void set_model(RenderObject *model);
    void set_modelScaling(glm::vec3 scaling);
    void set_parent(GameObject *parent);
    void set_position(glm::vec4 position);
    void set_rotation(glm::vec3 rotation);

    void addChild(GameObject *child);
    void translate(glm::vec4 offset);
    void rotate(glm::vec<3, float, (glm::qualifier)0> rotation);
    void render();
};

#endif // GAMEOBJECT_HEADER
