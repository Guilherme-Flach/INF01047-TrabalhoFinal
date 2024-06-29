#ifndef CAMERA_HEADER
#define CAMERA_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cmath>

class Camera : public GameObject {

  protected:
    Camera(glm::vec4 position);

    static glm::vec4 DEFAULT_UP_VECTOR;

    glm::vec4 up_vector;

  public:
    virtual glm::vec4 get_view() = 0;
    glm::vec4 get_up_vector();
    glm::vec4 get_u_vector();
    glm::vec4 get_v_vector();
    glm::vec4 get_w_vector();
    glm::mat4 get_viewMatrix();

    void set_up_vector(glm::vec4 up_vector);
};

class LookAtCamera : public Camera {

    glm::vec4 target;

  public:
    LookAtCamera(glm::vec4 position, glm::vec4 target);

    glm::vec4 get_target();

    void set_target(glm::vec4 target);
    glm::vec4 get_view() override;
};

class FreeCamera : public Camera {

    float phi, theta;

  public:
    FreeCamera(glm::vec4 position, float phi, float theta);

    void increase_theta(float amount);
    void increase_phi(float amount);

    float get_phi();
    float get_theta();

    void set_phi(float phi);
    void set_theta(float theta);
    glm::vec4 get_view() override;
};
#endif // CAMERA_HEADER
