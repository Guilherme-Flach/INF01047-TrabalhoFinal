#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "glm/ext/vector_float3.hpp"
#include "renderObject.hpp"

#include <vector>

class GameObject {

  protected:
    glm::vec4 position;
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 modelScaling = {1.0f, 1.0f, 1.0f};
    std::vector<GameObject> children;
    GameObject* parent = nullptr;
    RenderObject* model;


  public:
    GameObject(glm::vec4 position);
    glm::vec4 get_position();
    glm::mat4 get_model_matrix();
    std::vector<glm::vec4> get_vertices();
    std::vector<int> get_indices();

    void set_position(glm::vec4 position);
    void set_model(RenderObject* model);
    void set_rotation(glm::vec3 rotation);
    void set_modelScaling(glm::vec3 scaling);

    void addChild(GameObject child);
    void translate(glm::vec4 offset);
    void rotate(glm::vec3 rotation);
    void render(int program_id);
};

#endif // GAMEOBJECT_HEADER
