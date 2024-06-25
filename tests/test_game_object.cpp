#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include <gtest/gtest.h>
#include <cmath>

TEST(GameObject, TestGlobalPosition) {
    auto parent = GameObject(glm::vec4(15.0f, 9.0f, 5.0f, 1.0f));
    auto first_child = GameObject(glm::vec4(3.0f, 5.0f, 21.0f, 1.0f));
    auto second_child = GameObject(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    parent.addChild(&first_child);
    first_child.addChild(&second_child);

    auto global_position_parent = parent.get_global_position();
    auto global_position_first_child = first_child.get_global_position();
    auto global_position_second_child = second_child.get_global_position();

    ASSERT_FLOAT_EQ(parent.get_position().x, global_position_parent.x);
    ASSERT_FLOAT_EQ(parent.get_position().y, global_position_parent.y);
    ASSERT_FLOAT_EQ(parent.get_position().z, global_position_parent.z);

    ASSERT_FLOAT_EQ(parent.get_position().x + first_child.get_position().x,
                    global_position_first_child.x);
    ASSERT_FLOAT_EQ(parent.get_position().y + first_child.get_position().y,
                    global_position_first_child.y);
    ASSERT_FLOAT_EQ(parent.get_position().z + first_child.get_position().z,
                    global_position_first_child.z);

    ASSERT_FLOAT_EQ(parent.get_position().x + first_child.get_position().x +
                        second_child.get_position().x,
                    global_position_second_child.x);
    ASSERT_FLOAT_EQ(parent.get_position().y + first_child.get_position().y +
                        second_child.get_position().y,
                    global_position_second_child.y);
    ASSERT_FLOAT_EQ(parent.get_position().z + first_child.get_position().z +
                        second_child.get_position().z,
                    global_position_second_child.z);
}

TEST(GameObject, TestBasis) {
    auto parent = GameObject(glm::vec4(15.0f, 9.0f, 5.0f, 1.0f));
    auto first_child = GameObject(glm::vec4(3.0f, 5.0f, 21.0f, 1.0f));

    parent.addChild(&first_child);

    parent.rotate({M_PI / 2, 0.0f, 0.0f});

    ASSERT_FLOAT_EQ(parent.get_basis()[0].x,
                    first_child.get_global_basis()[0].x);
    ASSERT_FLOAT_EQ(parent.get_basis()[0].y,
                    first_child.get_global_basis()[0].y);
    ASSERT_FLOAT_EQ(parent.get_basis()[0].z,
                    first_child.get_global_basis()[0].z);

    auto a = first_child.get_global_basis();
    ASSERT_FLOAT_EQ(parent.get_basis()[1].x,
                    first_child.get_global_basis()[1].x);
    ASSERT_FLOAT_EQ(parent.get_basis()[1].y,
                    first_child.get_global_basis()[1].y);
    ASSERT_FLOAT_EQ(parent.get_basis()[1].z,
                    first_child.get_global_basis()[1].z);

    ASSERT_FLOAT_EQ(parent.get_basis()[2].x,
                    first_child.get_global_basis()[2].x);
    ASSERT_FLOAT_EQ(parent.get_basis()[2].y,
                    first_child.get_global_basis()[2].y);
    ASSERT_FLOAT_EQ(parent.get_basis()[2].z,
                    first_child.get_global_basis()[2].z);
}

TEST(GameObject, TestModelMatrix) {
    auto parent = GameObject(glm::vec4(15.0f, 9.0f, 5.0f, 1.0f));
    auto first_child = GameObject(glm::vec4(3.0f, 5.0f, 21.0f, 1.0f));

    parent.addChild(&first_child);

    ASSERT_FLOAT_EQ(parent.get_model_matrix()[3].x, parent.get_position().x);
    ASSERT_FLOAT_EQ(parent.get_model_matrix()[3].y, parent.get_position().y);
    ASSERT_FLOAT_EQ(parent.get_model_matrix()[3].z, parent.get_position().z);
    ASSERT_FLOAT_EQ(parent.get_model_matrix()[3].w, 1.0f);

    ASSERT_FLOAT_EQ(first_child.get_model_matrix()[3].x,
                    parent.get_position().x + first_child.get_position().x);
    ASSERT_FLOAT_EQ(first_child.get_model_matrix()[3].y,
                    parent.get_position().y + first_child.get_position().y);
    ASSERT_FLOAT_EQ(first_child.get_model_matrix()[3].z,
                    parent.get_position().z + first_child.get_position().z);
    ASSERT_FLOAT_EQ(first_child.get_model_matrix()[3].w, 1.0f);

    ASSERT_EQ(first_child.get_model_matrix()[0], parent.get_model_matrix()[0]);
    ASSERT_EQ(first_child.get_model_matrix()[1], parent.get_model_matrix()[1]);
    ASSERT_EQ(first_child.get_model_matrix()[2], parent.get_model_matrix()[2]);
}
