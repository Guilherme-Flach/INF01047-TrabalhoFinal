#ifndef CAMERA_HEADER
#define CAMERA_HEADER
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cmath>

class Camera {

  protected:
    glm::vec4 position;

    Camera(glm::vec4 position);

    static glm::vec4 DEFAULT_UP_VECTOR;

  public:
    glm::vec4 get_position();
    virtual glm::vec4 get_view() = 0;
    glm::mat4 get_viewMatrix();

    void set_position(glm::vec4 position);
    void translate(glm::vec4 offset);
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

    float get_phi();
    float get_theta();

    void set_phi(float phi);
    void set_theta(float theta);
    glm::vec4 get_view() override;
};
#endif // CAMERA_HEADER
