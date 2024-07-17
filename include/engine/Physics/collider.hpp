#include "RTree.h"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <vector>
struct CollisionData {
  bool isColliding;
  glm::vec4 collisionPoint;

  CollisionData(bool isColliding);
  CollisionData(bool isColliding, glm::vec4 collisionPoint);
};

class Collider : protected GameObject {

  private:
    int id = -1;

  public:
    enum ColliderType {
        BOX_COLIDER,
        SPHERE_COLLIDER,
        RAY_COLLIDER
      };

    Collider(glm::vec4 center,  ColliderType colliderType);

  protected:
    glm::vec4 center;
    ColliderType colliderType;

    virtual bool test_collision(Collider &other);

  public:
    void update_id(int new_id);
    int get_id();
    virtual glm::vec4 get_min();
    virtual glm::vec4 get_max();
};

class BoxCollider : public Collider {

  public:
    BoxCollider(glm::vec4 center, float x, float y, float z);

  protected:
    float x, y, z;
    glm::vec4 vertices[4];

    bool test_collision(Collider &other) override;

  public:
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
};

class SphereCollider : Collider {

  public:
    SphereCollider(glm::vec4 center, float radius);

  protected:
    float radius;

    bool test_collision(Collider &other) override;

  public:
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
};

class RaycastCollider : Collider {

  public:
    RaycastCollider(glm::vec4 start, glm::vec4 direction);

  protected:
    glm::vec4 displacement;

  bool test_collision(Collider &other) override;

  public:
    glm::vec4 get_min() override;
    glm::vec4 get_max() override;
};

class CollisionsManager {

  private:
    /* Adapted from https://superliminal.com/sources/#C_Code */
    RTree<int, float, 3, float> collision_hierarchy;

    std::atomic<int> id{};
    std::map<int, Collider &> colliders;

  public:
    void add_collider(Collider &collider);
    void remove_collider(Collider &collider);
    void search_collider(Collider &collider, std::vector<int> &found);
};
