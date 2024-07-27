#ifndef CAMERA_HEADER
#define CAMERA_HEADER
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif


class Camera : public GameObject {

  protected:
    static glm::vec4 DEFAULT_UP_VECTOR;

    glm::vec4 up_vector;
    GameObject *target;

    float fov = M_PI_2;
    float nearPlane = -0.1f;
    float farPlane = -200.0f;

  public:
    Camera(glm::vec4 position, GameObject *target);

    glm::vec4 get_view();
    glm::vec4 get_up_vector();
    glm::vec4 get_u_vector();
    glm::vec4 get_v_vector();
    glm::vec4 get_w_vector();
    glm::mat4 get_viewMatrix();
    float get_fov();
    float get_nearPlane();
    float get_farPlane();


    GameObject *get_target();

    void update(GLfloat deltaTime);

    void set_up_vector(glm::vec4 up_vector);
    void set_target(GameObject *target);
    void set_fov(GLfloat fov);
    void set_nearPlane(float nearPlane);
    void set_farPlane(float farPlane);
};
#endif // CAMERA_HEADER
