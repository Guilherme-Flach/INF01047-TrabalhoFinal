#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER
#include <functional>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/matrix_float4x4.hpp"

enum GameObjectType { STANDARD, COLLIDER };

class GameObject {

  protected:
    GameObjectType type;
    glm::mat4 model_matrix;
    std::vector<GameObject *> children;
    void set_parent(GameObject &parent);
    void remove_parent();

    GameObject *parent;
    Texture *texture;
    Model3D *model;
    glm::vec3 modelScaling;
    bool shouldRender;

    std::function<void(GLfloat deltaTime)> onUpdate;

  public:
    GameObject(GameObjectType type, glm::vec4 position);
    GameObjectType get_type();
    std::vector<GameObject *> *get_children();
    GameObject *get_parent();
    glm::vec4 get_position();
    glm::vec4 get_global_position();
    glm::mat4 get_model_matrix();
    glm::mat3x4 get_global_basis();
    glm::mat3x4 get_basis();
    Model3D *get_model();
    Texture *get_texture();
    glm::vec3 get_modelScaling();
    bool get_isRenderable();

    void set_model(Model3D *model);
    void set_texture(Texture *texture);
    void set_modelScaling(glm::vec3 scaling);
    void set_modelScaling(float scaling);
    void set_onUpdate(std::function<void(GLfloat deltaTime)> updateFunction);

    void set_position(glm::vec4 position);

    void update(GLfloat deltaTime);

    void addChild(GameObject &child);
    void translate(glm::vec4 offset);
    void rotate(glm::vec3 rotation);
    void rotate(float angle, glm::vec4 axis);
};

#endif // GAMEOBJECT_HEADER
