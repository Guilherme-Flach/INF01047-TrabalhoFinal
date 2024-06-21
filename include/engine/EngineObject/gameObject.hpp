#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engineObject.hpp"
#include "engine/loader.hpp"
#include "glm/ext/vector_float3.hpp"
#include "../Rendering/model3D.hpp"

#include <vector>

class GameObject : public EngineObject {

  protected:
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 modelScaling = {1.0f, 1.0f, 1.0f};
    Model3D *model;

  public:
    GameObject(glm::vec4 position, GLuint program_id);
    Model3D *get_model();
    glm::vec3 get_modelScaling();
    glm::vec3 get_rotation();
    glm::vec4 get_position();
    glm::mat4 get_model_matrix();

    void set_model(Model3D *model);
    void set_modelScaling(glm::vec3 scaling);
    void set_position(glm::vec4 position);
    void set_rotation(glm::vec3 rotation);

    void rotate(glm::vec<3, float, (glm::qualifier)0> rotation);
};

#endif // GAMEOBJECT_HEADER
