#ifndef COLLIDER_HEADER
#define COLLIDER_HEADER

#include "RTree.h"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include <atomic>
#include <functional>

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
    PhysicsObject *physicsObjectParent;
    ColliderType colliderType;

  public:
    PhysicsObject *get_parent();
    virtual ~Collider() = default;
    Collider(PhysicsObject *parent, glm::vec4 position,
             ColliderType colliderType);
    void update_id(int new_id);
    int get_id();
    ColliderType get_collider_type();
    virtual glm::vec4 get_min() = 0;
    virtual glm::vec4 get_max() = 0;
    virtual CollisionData test_collision(Collider &other) = 0;
};

class SphereCollider : public Collider {

  public:
    SphereCollider(PhysicsObject *parent, glm::vec4 center, float radius);

  protected:
    float radius;

  public:
    float get_radius();
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
    CollisionData test_collision(Collider &other) override;
};

class BoxCollider : public Collider {

  public:
    BoxCollider(PhysicsObject *parent, glm::vec4 center, float width, float height,
                float depth);

  protected:
    float width, height, depth;

  public:
    CollisionData test_sphere(SphereCollider &sphere);
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
    CollisionData test_collision(Collider &other) override;
};

class RaycastCollider : public Collider {

  public:
    RaycastCollider(PhysicsObject *parent, glm::vec4 start,
                    glm::vec4 direction);

  protected:
    glm::vec4 displacement;

  public:
    CollisionData test_sphere(SphereCollider sphere);
    glm::vec4 get_displacement();
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
    CollisionData test_collision(Collider &other) override;
};

class CollisionsManager {

  private:
    /* FONTE: Adapted from https://superliminal.com/sources/#C_Code */

    std::atomic<int> id{};

  public:
    RTree<int, float, 3, float> collision_hierarchy;
    std::map<int, Collider *> colliders;
    void add_object(PhysicsObject &object);
    void add_or_update_collider(Collider &collider);
    void remove_collider(Collider &collider);
    void search_collider(Collider *collider,
                         std::function<void(Collider *)> on_hit);
    std::pair<int, Collider *> get_colliders();
    void update_colliders();
    void handle_collisions(GLfloat deltaTime);
};

#endif // COLLIDER_HEADER
