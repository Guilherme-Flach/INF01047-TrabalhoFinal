#include "glm/ext/vector_float4.hpp"
#include <cmath>

class Camera {

  protected:
    glm::vec4 position;
    glm::vec4 up;

    Camera(glm::vec4 position, glm::vec4 up) : position(position), up(up) {}

    constexpr static const auto DEFAULT_UP_VECTOR =
        glm::vec4(0.0, 1.0, 0.0, 0.0);

  public:
    glm::vec4 get_position() { return position; }
    virtual glm::vec4 get_view();
    glm::vec4 get_up() { return up; }

    void set_position(glm::vec4 position) { this->position = position; }
    void translate(glm::vec4 offset) { position += offset; }
    void set_up(glm::vec4 up) { this->up = up; }
};

class LookAtCamera : public Camera {

    glm::vec4 target;

  public:
    LookAtCamera(glm::vec4 position, glm::vec4 target)
        : Camera(position, DEFAULT_UP_VECTOR) {}

    glm::vec4 get_target() { return target; }

    void set_target(glm::vec4 target) { this->target = target; }
    glm::vec4 get_view() override { return target - position; }
};

class FreeCamera : public Camera {

    float phi, theta;

  public:
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
