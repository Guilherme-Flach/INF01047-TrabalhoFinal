#include "engine/EngineObject/engineObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include <gtest/gtest.h>

TEST(Matrices, LocalToGlobalCoordinates) {
    Basis localBasis = {glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
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
    Basis localBasis = {glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
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
    Basis defaultBasis = {glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
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
