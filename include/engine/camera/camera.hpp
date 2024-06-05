#include "glm/ext/vector_float4.hpp"
#include <cmath>

class Camera {

  protected:
    glm::vec4 position;

    Camera(glm::vec4 position) : position(position) {}

    inline static const glm::vec4 DEFAULT_UP_VECTOR = {0.0, 1.0, 0.0, 0.0};

  public:
    glm::vec4 get_position() { return position; }
    virtual glm::vec4 get_view() = 0;

    void set_position(glm::vec4 position) { this->position = position; }
    void translate(glm::vec4 offset) { position += offset; }
};

class LookAtCamera : public Camera {

    glm::vec4 target;

  public:
    LookAtCamera(glm::vec4 position, glm::vec4 target)
        : Camera(position), target(target) {}

    glm::vec4 get_target() { return target; }

    void set_target(glm::vec4 target) { this->target = target; }
    glm::vec4 get_view() override { return target - position; }
};

class FreeCamera : public Camera {

    float phi, theta;

  public:
    FreeCamera(glm::vec4 position, float phi, float theta)
        : Camera(position), phi(phi), theta(theta) {}

    float get_phi() { return phi; }
    float get_theta() { return theta; }

    void set_phi(float phi) { this->phi = phi; }
    void set_theta(float theta) { this->theta = theta; }
    glm::vec4 get_view() override {
        float rho = 1.0;
        float z = rho * std::cos(phi) * std::cos(theta);
        float x = rho * std::cos(phi) * std::sin(theta);
        float y = rho * std::sin(phi);
        return glm::vec4(x, y, z, 0.0);
    }
};
