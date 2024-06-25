#include "engine/EngineObject/camera/camera.hpp"
#include "engine/loader.hpp"
#include "GLFW/glfw3.h"
#include <functional>
#include <iostream>

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";

    auto loader = Loader(width, height, title);

    std::vector<GLfloat> vertices = std::vector<GLfloat>({
        -0.5f, 0.5f,  0.5f,  1.0f, // posição do vértice 0
        -0.5f, -0.5f, 0.5f,  1.0f, // posição do vértice 1
        0.5f,  -0.5f, 0.5f,  1.0f, // posição do vértice 2
        0.5f,  0.5f,  0.5f,  1.0f, // posição do vértice 3
        -0.5f, 0.5f,  -0.5f, 1.0f, // posição do vértice 4
        -0.5f, -0.5f, -0.5f, 1.0f, // posição do vértice 5
        0.5f,  -0.5f, -0.5f, 1.0f, // posição do vértice 6
        0.5f,  0.5f,  -0.5f, 1.0f, // posição do vértice 7
    });

    std::vector<GLfloat> colors = std::vector<GLfloat>({
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 0
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 1
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 2
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 3
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 4
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 5
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 6
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 7
    });

    std::vector<GLuint> indices = std::vector<GLuint>({
        0, 1, 2, // triângulo 1
        7, 6, 5, // triângulo 2
        3, 2, 6, // triângulo 3
        4, 0, 3, // triângulo 4
        4, 5, 1, // triângulo 5
        1, 5, 6, // triângulo 6
        0, 2, 3, // triângulo 7
        7, 5, 4, // triângulo 8
        3, 6, 7, // triângulo 9
        4, 3, 7, // triângulo 10
        4, 1, 0, // triângulo 11
        1, 6, 2, // triângulo 12
    });

    glm::vec4 position = {2.0, 2.0, 2.0, 1.0};
    glm::vec4 target = {0.0, 0.0, 0.0, 1.0};
    LookAtCamera camera = LookAtCamera(position, target);

    loader.add_camera(camera);
    loader.set_active_camera(&camera);

    Model3D cuboRender = Model3D(vertices, indices, colors, GL_TRIANGLES);
    GameObject cubo1 = GameObject({0.0f, 0.0f, 0.0f, 1.0f});
    cubo1.set_model(cuboRender);

    GameObject romano = GameObject({0.0f, 0.0f, 0.0f, 1.0f});

    romano.addChild(cubo1);

    loader.add_game_object(romano);

    addKeymap({GLFW_KEY_W, GLFW_PRESS}, [&cubo1, &camera]() {
        auto distance = cubo1.get_position() - camera.get_position();
        distance *= 0.1f;
        camera.translate(distance);
    });

    addKeymap({GLFW_KEY_S, GLFW_PRESS}, [&cubo1, &camera]() {
        auto distance = cubo1.get_position() - camera.get_position();
        distance *= 0.1f;
        camera.translate(-distance);
    });

    loader.start([&cubo1]() { cubo1.rotate({0.0f, 1.0f, 0.0f}); });
    return 0;
}
