#ifndef COLLIDER_HEADER
#define COLLIDER_HEADER

#include "RTree.h"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include <atomic>
#include <set>

struct CollisionData {
    bool isColliding;
    glm::vec4 collisionPoint;

    CollisionData(bool isColliding);
    CollisionData(bool isColliding, glm::vec4 collisionPoint);
};

enum ColliderType { BOX_COLLIDER, SPHERE_COLLIDER, RAY_COLLIDER };

class Collider : public GameObject {

  private:
    int id = -1;

  protected:
    glm::vec4 center;
    ColliderType colliderType;

  public:
    Collider(glm::vec4 center, ColliderType colliderType);
    void update_id(int new_id);
    int get_id();
    ColliderType get_collider_type();
    virtual glm::vec4 get_min() = 0;
    virtual glm::vec4 get_max() = 0;
};

class BoxCollider : public Collider {

  public:
    BoxCollider(glm::vec4 center, float x, float y, float z);

  protected:
    float x, y, z;
    glm::vec4 vertices[4];

  public:
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
};

class SphereCollider : public Collider {

  public:
    SphereCollider(glm::vec4 center, float radius);

  protected:
    float radius;

  public:
    float get_radius();
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
};

class RaycastCollider : public Collider {

  public:
    RaycastCollider(glm::vec4 start, glm::vec4 direction);

  protected:
    glm::vec4 displacement;

  public:
    CollisionData test_sphere(SphereCollider sphere);
    glm::vec4 get_displacement();
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
};

class CollisionsManager {

  private:
    /* FONTE: Adapted from https://superliminal.com/sources/#C_Code */
    RTree<int, float, 3, float> collision_hierarchy;

    std::atomic<int> id{};
    std::map<int, Collider *> colliders;

  public:
    void add_collider(Collider &collider);
    void remove_collider(Collider &collider);
    void search_collider(Collider &collider, std::set<int> &found);
    CollisionData test_collision(Collider &first, Collider &second);
};

#endif // COLLIDER_HEADER