#include "engine/Physics/collider.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cmath>

Collider::Collider(glm::vec4 center) : GameObject(center) {}

void Collider::update_id(int new_id) { this->id = new_id; }
int Collider::get_id() { return this->id; }

BoxCollider::BoxCollider(glm::vec4 center, float x, float y, float z)
    : Collider(center), x(x), y(y), z(z) {
    this->vertices[0] =
        this->center + glm::vec4(this->x, this->y, this->z, 0.0f);
    this->vertices[1] =
        this->center + glm::vec4(this->x, -this->y, this->z, 0.0f);
    this->vertices[2] =
        this->center + glm::vec4(this->x, this->y, -this->z, 0.0f);
    this->vertices[3] =
        this->center + glm::vec4(this->x, -this->y, -this->z, 0.0f);

    this->vertices[4] =
        this->center + glm::vec4(-this->x, this->y, this->z, 0.0f);
    this->vertices[5] =
        this->center + glm::vec4(-this->x, -this->y, this->z, 0.0f);
    this->vertices[6] =
        this->center + glm::vec4(-this->x, this->y, -this->z, 0.0f);
    this->vertices[7] =
        this->center + glm::vec4(-this->x, -this->y, -this->z, 0.0f);
}

glm::vec4 BoxCollider::get_min() {
    float min_x = std::fmin(
        vertices[0].x,
        std::fmin(vertices[1].x, std::fmin(vertices[2].x, vertices[3].x)));
    float min_y = std::fmin(
        vertices[0].y,
        std::fmin(vertices[1].y, std::fmin(vertices[2].y, vertices[3].y)));
    float min_z = std::fmin(
        vertices[0].z,
        std::fmin(vertices[1].z, std::fmin(vertices[2].z, vertices[3].z)));
    return glm::vec4(min_x, min_y, min_z, 1.0f);
}

glm::vec4 BoxCollider::get_max() {
    float max_x = std::fmax(
        vertices[0].x,
        std::fmax(vertices[1].x, std::fmax(vertices[2].x, vertices[3].x)));
    float max_y = std::fmax(
        vertices[0].y,
        std::fmax(vertices[1].y, std::fmax(vertices[2].y, vertices[3].y)));
    float max_z = std::fmax(
        vertices[0].z,
        std::fmax(vertices[1].z, std::fmax(vertices[2].z, vertices[3].z)));
    return glm::vec4(max_x, max_y, max_z, 1.0f);
}

glm::vec4 SphereCollider::get_min() {
    return this->center -
           glm::vec4(this->radius, this->radius, this->radius, 0.0f);
}

glm::vec4 SphereCollider::get_max() {
    return this->center +
           glm::vec4(this->radius, this->radius, this->radius, 0.0f);
}

glm::vec4 RaycastCollider::get_min() { return this->center; }

glm::vec4 RaycastCollider::get_max() {
    return this->center + this->displacement;
}

SphereCollider::SphereCollider(glm::vec4 center, float radius)
    : Collider(center), radius(radius) {}

RaycastCollider::RaycastCollider(glm::vec4 start, glm::vec4 direction)
    : Collider(start), displacement(direction) {}

void CollisionsManager::add_collider(Collider &collider) {
    auto min_vec = collider.get_min();
    auto max_vec = collider.get_max();
    float min[3] = {min_vec.x, min_vec.y, min_vec.z};
    float max[3] = {max_vec.x, max_vec.y, max_vec.z};
    collider.update_id(this->id++);
    this->colliders[collider.get_id()] = collider;
    this->collision_hierarchy.Insert(min, max, collider.get_id());
}

void CollisionsManager::remove_collider(Collider &collider) {
    auto min_vec = collider.get_min();
    auto max_vec = collider.get_max();
    float min[3] = {min_vec.x, min_vec.y, min_vec.z};
    float max[3] = {max_vec.x, max_vec.y, max_vec.z};
    this->colliders.erase(collider.get_id());
    this->collision_hierarchy.Remove(min, max, collider.get_id());
}

void CollisionsManager::search_collider(Collider &collider,
                                        std::vector<int> &found) {
    auto min_vec = collider.get_min();
    auto max_vec = collider.get_max();
    float min[3] = {min_vec.x, min_vec.y, min_vec.z};
    float max[3] = {max_vec.x, max_vec.y, max_vec.z};
    this->collision_hierarchy.Search(min, max, found, nullptr);
}
