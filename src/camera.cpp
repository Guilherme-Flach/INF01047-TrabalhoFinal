#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/loader.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"

glm::vec4 Camera::DEFAULT_UP_VECTOR = {0.0, 1.0, 0.0, 0.0};

Camera::Camera(glm::vec4 position, glm::vec4 target)
    : GameObject(position), target(target), up_vector(DEFAULT_UP_VECTOR) {}

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

glm::vec4 Camera::get_target() { return target; }

void Camera::set_target(glm::vec4 target) { this->target = target; }

glm::vec4 Camera::get_view() { return target - get_global_position(); }

void Camera::update(GLfloat deltaTime) {
    GameObject::update(deltaTime);
}

