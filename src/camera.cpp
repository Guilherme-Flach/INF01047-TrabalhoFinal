#include "engine/EngineObject/camera/camera.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"

// Camera
Camera::Camera(glm::vec4 position) : GameObject(position), up_vector(DEFAULT_UP_VECTOR) {}

glm::vec4 Camera::DEFAULT_UP_VECTOR = {0.0, 1.0, 0.0, 0.0};

glm::mat4 Camera::get_viewMatrix() {
    return Matrix_Camera_View(get_global_position(), get_view(), get_up_vector());
}

glm::vec4 Camera::get_u_vector() {
    const glm::vec4 w_vector = this->get_w_vector();
    const glm::vec4 up_vector = this->up_vector;
    const glm::vec4 cross_up_w = crossproduct(up_vector, w_vector);
    return cross_up_w / norm(cross_up_w);

}

glm::vec4 Camera::get_v_vector() {
    return crossproduct(get_w_vector(), get_u_vector());
}

glm::vec4 Camera::get_w_vector() {
    const glm::vec4 view_vector = this->get_view();
    return -view_vector / norm(view_vector);
}

glm::vec4 Camera::get_up_vector() { return up_vector;}

void Camera::set_up_vector(glm::vec4 up_vector) { this->up_vector = up_vector;}

// LookAt
LookAtCamera::LookAtCamera(glm::vec4 position, glm::vec4 target)
    : Camera(position), target(target) {}

glm::vec4 LookAtCamera::get_target() { return target; }

glm::vec4 LookAtCamera::get_view() { return target - get_global_position(); }

void LookAtCamera::set_target(glm::vec4 target) { this->target = target; }

// Free
FreeCamera::FreeCamera(glm::vec4 position, float phi, float theta)
    : Camera(position), phi(phi), theta(theta) {}

float FreeCamera::get_phi() { return phi; }

float FreeCamera::get_theta() { return theta; }

void FreeCamera::increase_theta(float amount) { this->theta += amount; }

void FreeCamera::increase_phi(float amount) { this->phi += amount; }

void FreeCamera::set_phi(float phi) { this->phi = phi; }

void FreeCamera::set_theta(float theta) { this->theta = theta; }

glm::vec4 FreeCamera::get_view() {
    float rho = 1.0;
    float z = rho * std::cos(phi) * std::cos(theta);
    float x = rho * std::cos(phi) * std::sin(theta);
    float y = rho * std::sin(phi);
    return glm::vec4(x, y, z, 0.0);
}
