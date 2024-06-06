#include "engine/camera/camera.hpp"
#include "glm/ext/vector_float4.hpp"

Camera::Camera(glm::vec4 position) : position(position) {}

glm::vec4 Camera::DEFAULT_UP_VECTOR = {0.0, 1.0, 0.0, 0.0};

glm::vec4 Camera::get_position() { return position; }

void Camera::set_position(glm::vec4 position) { this->position = position; }

void Camera::translate(glm::vec4 offset) { position += offset; }

LookAtCamera::LookAtCamera(glm::vec4 position, glm::vec4 target)
    : Camera(position), target(target) {}

glm::vec4 LookAtCamera::get_target() { return target; }

void LookAtCamera::set_target(glm::vec4 target) { this->target = target; }

glm::vec4 LookAtCamera::get_view() { return target - position; }

FreeCamera::FreeCamera(glm::vec4 position, float phi, float theta)
    : Camera(position), phi(phi), theta(theta) {}

float FreeCamera::get_phi() { return phi; }

float FreeCamera::get_theta() { return theta; }

void FreeCamera::set_phi(float phi) { this->phi = phi; }

void FreeCamera::set_theta(float theta) { this->theta = theta; }

glm::vec4 FreeCamera::get_view() {
    float rho = 1.0;
    float z = rho * std::cos(phi) * std::cos(theta);
    float x = rho * std::cos(phi) * std::sin(theta);
    float y = rho * std::sin(phi);
    return glm::vec4(x, y, z, 0.0);
}