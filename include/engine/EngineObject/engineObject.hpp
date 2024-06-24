#ifndef ENGINEOBJECT_HEADER
#define ENGINEOBJECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
// #include <gtest/gtest_prod.h>
#include <vector>
#include "basis.hpp"
#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/matrix_float4x4.hpp"

class EngineObject {

  protected:
    // FRIEND_TEST(EngineObject, TestGlobalPosition);
    // FRIEND_TEST(EngineObject, TestBasis);
    // FRIEND_TEST(EngineObject, TestModelMatrix);

    glm::mat4 model;
    std::vector<EngineObject*> children;
    EngineObject *parent;
    EngineObject(glm::vec4 position);
    void set_parent(EngineObject *parent);

  public:
    std::vector<EngineObject*> *get_children();
    EngineObject *get_parent();
    glm::vec4 get_position();
    glm::vec4 get_global_position();
    glm::mat4 get_model_matrix();
    glm::mat3x4 get_global_basis();
    glm::mat3x4 get_basis();

    void set_position(glm::vec4 position);

    void addChild(EngineObject *child);
    void translate(glm::vec4 offset);
    void rotate(glm::vec3 rotation);
};

#endif // GAMEOBJECT_HEADER
