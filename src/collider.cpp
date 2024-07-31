#include "engine/Physics/collider.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cmath>
#include <iostream>

CollisionData::CollisionData(bool isColliding)
    : isColliding(isColliding), collisionPoint({0.0f, 0.0f, 0.0f, 0.0f}) {};
CollisionData::CollisionData(bool isColliding, glm::vec4 collisionPoint)
    : isColliding(isColliding), collisionPoint(collisionPoint) {};

Collider::Collider(glm::vec4 center, ColliderType colliderType)
    : GameObject(center), colliderType(colliderType) {}

void Collider::update_id(int new_id) { this->id = new_id; }
int Collider::get_id() { return this->id; }

ColliderType Collider::get_collider_type() { return this->colliderType; }

BoxCollider::BoxCollider(glm::vec4 center, float x, float y, float z)
    : Collider(center, ColliderType::BOX_COLLIDER), x(x), y(y), z(z) {
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
    : Collider(center, ColliderType::SPHERE_COLLIDER), radius(radius) {}

float SphereCollider::get_radius() { return this->radius; }

RaycastCollider::RaycastCollider(glm::vec4 start, glm::vec4 direction)
    : Collider(start, ColliderType::RAY_COLLIDER), displacement(direction) {}

void CollisionsManager::add_or_update_collider(Collider &collider) {
    auto min_vec = collider.get_min();
    auto max_vec = collider.get_max();
    float min[3] = {min_vec.x, min_vec.y, min_vec.z};
    float max[3] = {max_vec.x, max_vec.y, max_vec.z};
    auto found_index = this->colliders.find(collider.get_id());
    if (found_index != this->colliders.end()) {
        auto found_collider = found_index->second;
        auto found_min_vec = found_collider->get_min();
        auto found_max_vec = found_collider->get_max();
        float found_min[3] = {found_min_vec.x, found_min_vec.y,
                              found_min_vec.z};
        float found_max[3] = {found_max_vec.x, found_max_vec.y,
                              found_max_vec.z};
        this->collision_hierarchy.Remove(found_min, found_max,
                                         found_collider->get_id());
    } else {
        collider.update_id(this->id++);
    }
    this->colliders[collider.get_id()] = &collider;
    this->collision_hierarchy.Insert(min, max, collider.get_id());
}

void CollisionsManager::remove_collider(Collider &collider) {
    auto min_vec = collider.get_min();
    auto max_vec = collider.get_max();
    float min[3] = {min_vec.x, min_vec.y, min_vec.z};
    float max[3] = {max_vec.x, max_vec.y, max_vec.z};
    auto entry = this->colliders.find(collider.get_id());
    std::cout << entry->first << std::endl;
    if (entry != this->colliders.end())
        this->colliders.erase(this->colliders.find(collider.get_id()));
    this->collision_hierarchy.Remove(min, max, collider.get_id());
}

void CollisionsManager::search_collider(Collider &collider,
                                        std::set<int> &found) {
    auto min_vec = collider.get_min();
    auto max_vec = collider.get_max();
    float min[3] = {min_vec.x, min_vec.y, min_vec.z};
    float max[3] = {max_vec.x, max_vec.y, max_vec.z};
    this->collision_hierarchy.Search(min, max, found);
}

glm::vec4 RaycastCollider::get_displacement() { return this->displacement; }

CollisionData RaycastCollider::test_sphere(SphereCollider sphere) {
    auto displacement = this->get_displacement();
    auto global_position = this->get_global_position();
    if (displacement.x == 0.0f && displacement.y == 0.0f &&
        displacement.z == 0.0f) {
        auto hasCollided = global_position.x * global_position.x +
                               global_position.y * global_position.y +
                               global_position.z * global_position.z ==
                           sphere.get_radius() * sphere.get_radius();
        return hasCollided ? CollisionData(true, global_position)
                           : CollisionData(false);
    }
    auto a = displacement.x * displacement.x + displacement.y * displacement.y +
             displacement.z * displacement.z;
    auto b = 2 * global_position.x * displacement.x +
             2 * global_position.y * displacement.y +
             2 * global_position.z * displacement.z;
    auto c = -(sphere.get_radius() * sphere.get_radius()) +
             global_position.x * global_position.x +
             global_position.y * global_position.y +
             global_position.z * global_position.z;
    auto delta = b * b - 4 * a * c;
    auto double_a = 2 * a;
    auto sqrt_delta = sqrt(delta);
    if (delta < 0) {
        return CollisionData(false);
    }
    auto first_offset = (-b + sqrt_delta) / double_a;
    if (delta == 0 && first_offset >= 0.0f && first_offset <= 1.0f) {
        return CollisionData(true, global_position + first_offset);
    }
    auto second_offset = (-b - sqrt_delta) / double_a;
    auto min_offset = fmin(first_offset, second_offset);
    if (min_offset < 0.0f || min_offset > 1.0f)
        return CollisionData(false);
    return CollisionData(true, global_position + min_offset);
}

CollisionData CollisionsManager::test_collision(Collider &collider) {
    std::set<int> found;
    this->search_collider(collider, found);
    for (int i = 0; i < found.size(); i++) {
        Collider *hit = this->colliders[*found.find(i)];
        if (hit->get_id() == collider.get_id())
            continue;
        std::cout << collider.get_id() << std::endl;
        std::cout << hit->get_id() << std::endl;
        auto data = test_collision(collider, *hit);
        if (data.isColliding)
            return data;
    }
    return CollisionData(false);
}

float squaredDistPointAABB(glm::vec4 sphere_center, glm::vec4 min,
                           glm::vec4 max) {
    auto check = [&](const double point, const double bmin,
                     const double bmax) -> double {
        double out = 0;
        double v = point;

        if (v < bmin) {
            double val = (bmin - v);
            out += val * val;
        }

        if (v > bmax) {
            double val = (v - bmax);
            out += val * val;
        }

        return out;
    };

    double sq = 0.0;

    sq += check(sphere_center.x, min.x, max.x);
    sq += check(sphere_center.y, min.y, max.y);
    sq += check(sphere_center.z, min.z, max.z);

    return sq;
}

CollisionData BoxCollider::test_sphere(SphereCollider &other) {
    return CollisionData(squaredDistPointAABB(other.get_global_position(),
                                              get_min(), get_max()) <=
                         other.get_radius() * other.get_radius());
}

CollisionData CollisionsManager::test_collision(Collider &first,
                                                Collider &second) {
    if (first.get_collider_type() == ColliderType::RAY_COLLIDER &&
        second.get_collider_type() == ColliderType::SPHERE_COLLIDER) {
        auto ray = dynamic_cast<RaycastCollider &>(first);
        auto sphere = dynamic_cast<SphereCollider &>(second);
        return ray.test_sphere(sphere);
    }
    if (first.get_collider_type() == ColliderType::SPHERE_COLLIDER &&
        second.get_collider_type() == ColliderType::RAY_COLLIDER) {
        auto ray = dynamic_cast<RaycastCollider &>(second);
        auto sphere = dynamic_cast<SphereCollider &>(first);
        return ray.test_sphere(sphere);
    }
    if (first.get_collider_type() == ColliderType::SPHERE_COLLIDER &&
        second.get_collider_type() == ColliderType::SPHERE_COLLIDER) {
        auto first_sphere = dynamic_cast<SphereCollider &>(first);
        auto second_sphere = dynamic_cast<SphereCollider &>(second);
        auto center = first_sphere.get_global_position();
        auto ray =
            RaycastCollider(center, second.get_global_position() - center);
        return ray.test_sphere(second_sphere);
    }
    if (first.get_collider_type() == ColliderType::BOX_COLLIDER &&
        second.get_collider_type() == ColliderType::SPHERE_COLLIDER) {
        auto first_aabb = dynamic_cast<BoxCollider &>(first);
        auto second_sphere = dynamic_cast<SphereCollider &>(second);
        return first_aabb.test_sphere(second_sphere);
    }
    if (first.get_collider_type() == ColliderType::SPHERE_COLLIDER &&
        second.get_collider_type() == ColliderType::BOX_COLLIDER) {
        auto first_sphere = dynamic_cast<SphereCollider &>(first);
        auto second_aabb = dynamic_cast<BoxCollider &>(second);
        return second_aabb.test_sphere(first_sphere);
    }
    return CollisionData(false);
}
