#include "engine/Physics/collider.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cmath>
#include <functional>
#include <iostream>

CollisionData::CollisionData(bool isColliding)
    : isColliding(isColliding), collisionPoint({0.0f, 0.0f, 0.0f, 0.0f}){};
CollisionData::CollisionData(bool isColliding, glm::vec4 collisionPoint)
    : isColliding(isColliding), collisionPoint(collisionPoint) {}

Collider::Collider(PhysicsObject *parent, glm::vec4 position,
                   ColliderType colliderType)
    : GameObject(GameObjectType::COLLIDER, position),
      physicsObjectParent(parent), colliderType(colliderType) {}

PhysicsObject *Collider::get_parent() { return this->physicsObjectParent; }

void Collider::update_id(int new_id) { this->id = new_id; }

int Collider::get_id() { return this->id; }

ColliderType Collider::get_collider_type() { return this->colliderType; }

BoxCollider::BoxCollider(PhysicsObject *parent, glm::vec4 center, float width,
                         float height, float depth)
    : Collider(parent, center, ColliderType::BOX_COLLIDER), width(width),
      height(height), depth(depth) {}

glm::vec4 BoxCollider::get_min() {
    glm::vec4 vertices[8];
    auto global_position = this->get_global_position();
    vertices[0] = global_position +
                  glm::vec4(this->width, this->height, this->depth, 0.0f);
    vertices[1] = global_position +
                  glm::vec4(this->width, -this->height, this->depth, 0.0f);
    vertices[2] = global_position +
                  glm::vec4(this->width, this->height, -this->depth, 0.0f);
    vertices[3] = global_position +
                  glm::vec4(this->width, -this->height, -this->depth, 0.0f);

    vertices[4] = global_position +
                  glm::vec4(-this->width, this->height, this->depth, 0.0f);
    vertices[5] = global_position +
                  glm::vec4(-this->width, -this->height, this->depth, 0.0f);
    vertices[6] = global_position +
                  glm::vec4(-this->width, this->height, -this->depth, 0.0f);
    vertices[7] = global_position +
                  glm::vec4(-this->width, -this->height, -this->depth, 0.0f);
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
    glm::vec4 vertices[8];
    auto global_position = this->get_global_position();
    vertices[0] = global_position +
                  glm::vec4(this->width, this->height, this->depth, 0.0f);
    vertices[1] = global_position +
                  glm::vec4(this->width, -this->height, this->depth, 0.0f);
    vertices[2] = global_position +
                  glm::vec4(this->width, this->height, -this->depth, 0.0f);
    vertices[3] = global_position +
                  glm::vec4(this->width, -this->height, -this->depth, 0.0f);

    vertices[4] = global_position +
                  glm::vec4(-this->width, this->height, this->depth, 0.0f);
    vertices[5] = global_position +
                  glm::vec4(-this->width, -this->height, this->depth, 0.0f);
    vertices[6] = global_position +
                  glm::vec4(-this->width, this->height, -this->depth, 0.0f);
    vertices[7] = global_position +
                  glm::vec4(-this->width, -this->height, -this->depth, 0.0f);
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
    return this->get_global_position() -
           glm::vec4(this->radius, this->radius, this->radius, 0.0f);
}

glm::vec4 SphereCollider::get_max() {
    return this->get_global_position() +
           glm::vec4(this->radius, this->radius, this->radius, 0.0f);
}

glm::vec4 RaycastCollider::get_min() { return this->get_global_position(); }

glm::vec4 RaycastCollider::get_max() {
    return this->get_global_position() + this->displacement;
}

SphereCollider::SphereCollider(PhysicsObject *parent, glm::vec4 center,
                               float radius)
    : Collider(parent, center, ColliderType::SPHERE_COLLIDER), radius(radius) {}

float SphereCollider::get_radius() { return this->radius; }

RaycastCollider::RaycastCollider(PhysicsObject *parent, glm::vec4 start,
                                 glm::vec4 direction)
    : Collider(parent, start, ColliderType::RAY_COLLIDER),
      displacement(direction) {}

void CollisionsManager::add_or_update_collider(Collider &collider) {
    auto min_vec = collider.get_min();
    auto max_vec = collider.get_max();
    float min[3] = {min_vec.x, min_vec.y, min_vec.z};
    float max[3] = {max_vec.x, max_vec.y, max_vec.z};
    auto found_index = this->colliders.find(collider.get_id());
    if (found_index != this->colliders.end()) {
        float min[3] = {-INFINITY, INFINITY, INFINITY};
        float max[3] = {INFINITY, INFINITY, INFINITY};
        this->collision_hierarchy.Remove(min, max, collider.get_id());
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
    if (entry != this->colliders.end())
        this->colliders.erase(this->colliders.find(collider.get_id()));
    this->collision_hierarchy.Remove(min, max, collider.get_id());
}

void CollisionsManager::search_collider(
    Collider *collider, std::function<void(Collider *)> on_hit) {
    auto min_vec = collider->get_min();
    auto max_vec = collider->get_max();
    float min[3] = {min_vec.x, min_vec.y, min_vec.z};
    float max[3] = {max_vec.x, max_vec.y, max_vec.z};
    std::set<int> nodes;
    this->collision_hierarchy.Search(min, max, nodes);
    std::set<int>::iterator node;
    for (node = nodes.begin(); node != nodes.end(); node++) {
        auto found_collider = this->colliders[*node];
        if (found_collider->get_id() == collider->get_id())
            continue;
        on_hit(found_collider);
    }
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

float squaredDistPointAABB(glm::vec4 sphere_center, glm::vec4 min,
                           glm::vec4 max) {
    auto calculate = [&](const float point, const float min,
                         const float max) -> float {
        if (point < min)
            return (min - point) * (min - point);
        if (point > max)
            return (point - max) * (point - max);
        return 0.0f;
    };

    float squaredDistance = 0.0f;
    squaredDistance += calculate(sphere_center.x, min.x, max.x);
    squaredDistance += calculate(sphere_center.y, min.y, max.y);
    squaredDistance += calculate(sphere_center.z, min.z, max.z);

    return squaredDistance;
}

CollisionData BoxCollider::test_sphere(SphereCollider &other) {
    auto placeholder =
        squaredDistPointAABB(other.get_global_position(), get_min(), get_max());
    return CollisionData(placeholder <=
                         other.get_radius() * other.get_radius());
}

CollisionData SphereCollider::test_collision(Collider &other) {
    switch (other.get_collider_type()) {
    case BOX_COLLIDER: {
        auto box_collider = static_cast<BoxCollider &>(other);
        return box_collider.test_sphere(*this);
    }
    case SPHERE_COLLIDER: {
        auto other_sphere = static_cast<SphereCollider &>(other);
        auto global_position = this->get_global_position();
        auto distance = other.get_global_position() - global_position;
        auto areColliding =
            glm::length(distance) <= other_sphere.get_radius() + radius;
        // if (areColliding) {
        //     RaycastCollider raycast_collider =
        //         RaycastCollider(physicsObjectParent, get_global_position(),
        //                         other.get_global_position());
        //     return raycast_collider.test_sphere(
        //         static_cast<SphereCollider &>(other));
        // }
        return CollisionData(areColliding);
    }
    case RAY_COLLIDER:
        auto raycast_collider = static_cast<RaycastCollider &>(other);
        return raycast_collider.test_sphere(*this);
    }
}

CollisionData BoxCollider::test_collision(Collider &other) {
    switch (other.get_collider_type()) {
    case BOX_COLLIDER:
        break;
    case SPHERE_COLLIDER: {
        return test_sphere(static_cast<SphereCollider &>(other));
    }
    case RAY_COLLIDER:
        break;
    }
}

CollisionData RaycastCollider::test_collision(Collider &other) {
    switch (other.get_collider_type()) {
    case BOX_COLLIDER:
    case SPHERE_COLLIDER: {
        return test_sphere(static_cast<SphereCollider &>(other));
    }
    case RAY_COLLIDER:
        break;
    }
}

void CollisionsManager::update_colliders() {
    std::map<int, Collider *>::iterator node;
    for (node = this->colliders.begin(); node != this->colliders.end();
         node++) {
        add_or_update_collider(*node->second);
    }
}

void CollisionsManager::handle_collisions(GLfloat deltaTime) {
    for (std::map<int, Collider *>::iterator node = colliders.begin();
         node != colliders.end(); node++) {
        auto collider = node->second;
        search_collider(collider, [&](Collider *hit) {
            const CollisionData collisionData = collider->test_collision(*hit);
            if (collisionData.isColliding) {
                auto first_object =
                    static_cast<PhysicsObject *>(collider->get_parent());
                auto second_object =
                    static_cast<PhysicsObject *>(hit->get_parent());
                first_object->handle_collision(collisionData.collisionPoint,
                                               *second_object, deltaTime);
            }
        });
    }
}

void CollisionsManager::add_object(PhysicsObject &object) {
    for (std::vector<GameObject *>::iterator node =
             object.get_children()->begin();
         node != object.get_children()->end(); node++) {
        if ((*node)->get_type() != GameObjectType::COLLIDER)
            continue;
        auto collider = static_cast<Collider *>(*node);
        add_or_update_collider(*collider);
    }
}
