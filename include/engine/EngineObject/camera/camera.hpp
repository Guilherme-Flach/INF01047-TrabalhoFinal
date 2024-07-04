#ifndef CAMERA_HEADER
#define CAMERA_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cmath>

class Camera : public GameObject {

  protected:
    static glm::vec4 DEFAULT_UP_VECTOR;

    glm::vec4 up_vector;
    glm::vec4 target;

  public:
    Camera(glm::vec4 position, glm::vec4 target);

    glm::vec4 get_view();
    glm::vec4 get_up_vector();
    glm::vec4 get_u_vector();
    glm::vec4 get_v_vector();
    glm::vec4 get_w_vector();
    glm::mat4 get_viewMatrix();

    glm::vec4 get_target();

    void set_up_vector(glm::vec4 up_vector);
    void set_target(glm::vec4 target);

};
#endif // CAMERA_HEADER
