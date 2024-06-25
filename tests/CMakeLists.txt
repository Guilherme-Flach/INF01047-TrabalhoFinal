cmake_minimum_required(VERSION 3.14)
project(LAB_FCG VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(
  tests
  test_matrices.cpp
  test_game_object.cpp
  ../src/camera.cpp
  ../src/gameObject.cpp
  ../src/matrices.cpp
  ../src/glad.c
  ../src/model3D.cpp
)
target_link_libraries(
  tests
  GTest::gtest_main
  ${PROJECT_SOURCE_DIR}/../include
)
target_include_directories(tests BEFORE PRIVATE ${PROJECT_SOURCE_DIR}/../include)

include(GoogleTest)
gtest_discover_tests(tests)
