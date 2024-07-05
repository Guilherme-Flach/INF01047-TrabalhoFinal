#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/player.hpp"
#include "engine/Rendering/defaultModels.hpp"
#include "engine/Rendering/model3D.hpp"
#include "engine/loader.hpp"
#include "engine/Input/inputHandler.hpp"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
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
    glm::vec4 origin = {0.0, 0.0, 0.0, 1.0};
    Camera cameraLookAt = Camera(position, origin);

    Camera cameraFree =
        Camera({0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f});
    GameObject target = GameObject({{1.0f, 0.0f, 0.0f, 1.0f}});
    cameraFree.addChild(target);
    cameraFree.set_onUpdate([&target, &cameraFree](GLfloat deltaTime) -> void {
        cameraFree.set_target(target.get_global_position());
    });

    GameObject romano = GameObject({0.0f, 0.0f, 0.0f, 1.0f});
    romano.set_modelScaling({10, 10, 10});

    PhysicsObject physObj = PhysicsObject({1.0f, 1.0f, 1.0f, 1.0f}, 1.0f);

    Model3D cuboModel = Model3D(vertices, indices, colors, GL_TRIANGLES);
    Model3D wireCubeModel = WireCube();
    Model3D dotModel = DotModel();
    Model3D noModel = NoModel();

    target.set_model(dotModel);
    cameraFree.set_model(noModel);

    Player gamer = Player({3.0f, 3.0f, 3.0f, 1.0f}, &cameraFree);
    gamer.set_model(wireCubeModel);
    gamer.addChild(cameraFree);

    GameObject cubo1 = GameObject({0.0f, 0.0f, 0.0f, 1.0f});
    cubo1.set_model(cuboModel);
    cubo1.set_modelScaling({0.5f, 1.0f, 0.5f});

    GameObject cubo2 = GameObject({1.0f, 0.0f, 0.0f, 1.0f});
    cubo2.set_model(cuboModel);
    cubo2.set_modelScaling({0.2f, 0.2f, 0.2f});
    cubo1.addChild(cubo2);

    GameObject cubo3 = GameObject({0.0f, 0.5f, 0.0f, 1.0f});
    cubo3.set_model(cuboModel);
    cubo3.set_modelScaling({0.2f, 0.2f, 0.2f});
    cubo2.addChild(cubo3);

    physObj.addChild(cubo1);
    physObj.addChild(cameraLookAt);

    GameObject cubo4 = GameObject({0.0f, 0.0f, 0.0f, 1.0f});
    cubo4.set_model(cuboModel);
    cubo4.set_modelScaling({0.3f, 0.3f, 0.3f});

    loader.add_game_object(romano);
    loader.add_game_object(physObj);
    loader.add_game_object(cubo4);
    loader.add_game_object(gamer);

    loader.add_camera(cameraLookAt);
    loader.add_camera(cameraFree);
    loader.set_active_camera(&cameraFree);

    InputHandler::addKeyMapping(GLFW_KEY_W, [&gamer](Action action) {
        const glm::vec4 direction = Player::FRONT;
        if (action == GLFW_PRESS) {
            gamer.increaseMovement(direction);
        } else if (action == GLFW_RELEASE) {
            gamer.increaseMovement(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_S,
                                [&gamer, &cameraFree](Action action) {
                                    const glm::vec4 direction = Player::BACK;
                                    if (action == GLFW_PRESS) {
                                        gamer.increaseMovement(direction);
                                    } else if (action == GLFW_RELEASE) {
                                        gamer.increaseMovement(-direction);
                                    }
                                });

    InputHandler::addKeyMapping(GLFW_KEY_A, [&gamer](Action action) {
        const glm::vec4 direction = Player::LEFT;
        if (action == GLFW_PRESS) {
            gamer.increaseMovement(direction);
        } else if (action == GLFW_RELEASE) {
            gamer.increaseMovement(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_D,
                                [&gamer, &cameraFree](Action action) {
                                    const glm::vec4 direction = Player::RIGHT;
                                    if (action == GLFW_PRESS) {
                                        gamer.increaseMovement(direction);
                                    } else if (action == GLFW_RELEASE) {
                                        gamer.increaseMovement(-direction);
                                    }
                                });

    InputHandler::addKeyMapping(GLFW_KEY_SPACE,
                                [&gamer, &cameraFree](Action action) {
                                    const glm::vec4 direction = Player::UP;
                                    if (action == GLFW_PRESS) {
                                        gamer.increaseMovement(direction);
                                    } else if (action == GLFW_RELEASE) {
                                        gamer.increaseMovement(-direction);
                                    }
                                });

    InputHandler::addKeyMapping(GLFW_KEY_LEFT_CONTROL,
                                [&gamer, &cameraFree](Action action) {
                                    const glm::vec4 direction = Player::DOWN;
                                    if (action == GLFW_PRESS) {
                                        gamer.increaseMovement(direction);
                                    } else if (action == GLFW_RELEASE) {
                                        gamer.increaseMovement(-direction);
                                    }
                                });

    // Rotation
    InputHandler::addKeyMapping(GLFW_KEY_UP, [&gamer](Action action) {
        const glm::vec4 rotation = Player::ROTATE_BACK;
        if (action == GLFW_PRESS) {
            gamer.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            gamer.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_DOWN, [&gamer](Action action) {
        const glm::vec4 rotation = Player::ROTATE_FRONT;
        if (action == GLFW_PRESS) {
            gamer.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            gamer.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_LEFT, [&gamer](Action action) {
        const glm::vec4 rotation = Player::ROTATE_LEFT;
        if (action == GLFW_PRESS) {
            gamer.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            gamer.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_RIGHT, [&gamer](Action action) {
        const glm::vec4 rotation = Player::ROTATE_RIGHT;
        if (action == GLFW_PRESS) {
            gamer.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            gamer.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_Q, [&gamer](Action action) {
        const glm::vec4 rotation = Player::ROTATE_CCLKWISE;
        if (action == GLFW_PRESS) {
            gamer.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            gamer.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_E, [&gamer](Action action) {
        const glm::vec4 rotation = Player::ROTATE_CLKWISE;
        if (action == GLFW_PRESS) {
            gamer.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            gamer.increaseRotationRate(-rotation);
        }
    });

    physObj.applyForce({0.5f, 0.00f, 0.5f, 0.0f});
    gamer.set_drag(0.6);

    loader.start([&cameraLookAt, &cubo1, &cubo3, &gamer, &cameraFree]() {
        gamer.update(Loader::get_delta_t());
        cameraFree.update(Loader::get_delta_t());
    });
    return 0;
}
