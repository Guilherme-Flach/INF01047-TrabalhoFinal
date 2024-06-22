#include "glm/ext/matrix_float3x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include <cmath>
#include <gtest/gtest.h>

TEST(Matrices, LocalToGlobalCoordinates) {
    glm::mat3x4 localBasis = {glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                              glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                              glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)};
    auto localOrigin = glm::vec4(13.0f, 15.0f, 27.0f, 1.0f);
    auto zero = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    auto result = Matrix_ToParentCoordinates(localOrigin, localBasis) * zero;
    ASSERT_FLOAT_EQ(localOrigin.x, result.x);
    ASSERT_FLOAT_EQ(localOrigin.y, result.y);
    ASSERT_FLOAT_EQ(localOrigin.z, result.z);
}

TEST(Matrices, GlobalToLocalCoordinates) {
    glm::mat3x4 localBasis = {glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                              glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                              glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)};
    auto localOrigin = glm::vec4(13.0f, 15.0f, 27.0f, 1.0f);
    auto zero = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    auto result =
        Matrix_ChangeCoordinates(zero, localOrigin, localBasis) * localOrigin;
    ASSERT_FLOAT_EQ(zero.x, result.x);
    ASSERT_FLOAT_EQ(zero.y, result.y);
    ASSERT_FLOAT_EQ(zero.z, result.z);
}

TEST(Matrices, LocalToLocalCoordinates) {
    glm::mat3x4 defaultBasis = {glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                                glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                                glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)};
    auto originSource = glm::vec4(13.0f, 15.0f, 27.0f, 1.0f);
    auto originTarget = glm::vec4(6.0f, 9.0f, 15.0f, 1.0f);
    auto zero = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    auto inGlobal =
        Matrix_ToParentCoordinates(originSource, defaultBasis) * zero;
    auto result =
        Matrix_ChangeCoordinates(zero, originTarget, defaultBasis) * inGlobal;
    ASSERT_FLOAT_EQ(originSource.x - originTarget.x, result.x);
    ASSERT_FLOAT_EQ(originSource.y - originTarget.y, result.y);
    ASSERT_FLOAT_EQ(originSource.z - originTarget.z, result.z);
}

TEST(Matrices, ChangeOfBasis) {
    glm::mat3x4 source = {glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f),
                          glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                          glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)};
    glm::mat3x4 target = Matrix_Orthogonalize(
        {glm::vec4(0.0f, 1.0f, 2.0f, 0.0f), glm::vec4(1.0f, 3.0f, 4.0f, 0.0f),
         glm::vec4(3.0f, 6.0f, 5.0f, 0.0f)});
    auto new_basis = Matrix_ChangeBasis(source, target);
    float tolerance = 1e-5;
    ASSERT_NEAR(0, dotproduct(target[0], target[1]), tolerance);
    ASSERT_NEAR(0, dotproduct(target[0], target[2]), tolerance);
    ASSERT_NEAR(0, dotproduct(target[1], target[2]), tolerance);

    ASSERT_NEAR(0, new_basis[0].x, tolerance);
    ASSERT_NEAR(-sqrt(5) / 3, new_basis[0].y, tolerance);
    ASSERT_NEAR(2.0 / 3, new_basis[0].z, tolerance);

    ASSERT_NEAR(1 / sqrt(5), new_basis[1].x, tolerance);
    ASSERT_NEAR(4 / (3 * sqrt(5)), new_basis[1].y, tolerance);
    ASSERT_NEAR(2.0 / 3, new_basis[1].z, tolerance);

    ASSERT_NEAR(-2 / sqrt(5), new_basis[2].x, tolerance);
    ASSERT_NEAR(2 / (3 * sqrt(5)), new_basis[2].y, tolerance);
    ASSERT_NEAR(1.0 / 3, new_basis[2].z, tolerance);
}
