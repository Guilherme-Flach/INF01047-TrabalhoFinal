#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/camera/dollyCamera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/player.hpp"
#include "engine/Rendering/defaultModels.hpp"
#include "engine/Rendering/model3D.hpp"
#include "engine/loader.hpp"
#include "engine/Input/inputHandler.hpp"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float4.hpp"
#include "interpolator.hpp"
#include "matrices.hpp"
#include <functional>
#include <iostream>

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";

    auto loader = Loader(width, height, title);

    Model3D cuboModel = DebugCubeModel();
    Model3D wireCubeModel = WireCube();
    Model3D dotModel = DotModel();
    Model3D noModel = NoModel();

    GameObject target = GameObject({{0.0f, 0.0f, 1.0f, 1.0f}});
    target.set_model(dotModel);

    Camera cameraFree = Camera({0.0f, 0.0f, 0.0f, 1.0f}, &target);
    cameraFree.set_model(noModel);
    cameraFree.addChild(target);

    Player gamer = Player({0.0f, 0.0f, -2.0f, 1.0f}, &cameraFree);
    gamer.set_model(wireCubeModel);
    gamer.addChild(cameraFree);

    const BezierPath_Quadratic cameraPath = {cameraFree.get_global_position(),
                                             {2.0f, 3.0f, 2.0f, 1.0f},
                                             {6.0f, 8.0f, 6.0f, 1.0f}};

    GameObject targetStartingPoint = GameObject({2.0f, -1.0f, -1.0f, 1.0f});
    targetStartingPoint.set_model(wireCubeModel);

    GameObject targetEndingPoint = GameObject({0.0f, 0.0f, 0.0f, 1.0f});
    targetEndingPoint.set_model(wireCubeModel);

    const BezierPath_Quadratic targetPath = {
        targetStartingPoint.get_global_position(),
        targetEndingPoint.get_global_position(),
        {-2.0f, -2.0f, -2.0f, 1.0f}};

    PhysicsObject physObj = PhysicsObject({0.0f, 0.0f, 0.0f, 1.0f}, 1.0f);
    physObj.set_model(cuboModel);

    const BezierPath_Quadratic lensPath = {
        {cameraFree.get_fov(), cameraFree.get_nearPlane(),
         cameraFree.get_farPlane(), 1.0f},
        {0.6*M_PI, cameraFree.get_nearPlane(), -50.0f, 1.0f},
        {0.8*M_PI, cameraFree.get_nearPlane(), -50.0f, 1.0f}};

    DollyCamera dollyCamera =
        DollyCamera(cameraPath, 1.2f, &physObj, targetPath, 1.0f, lensPath);
    
    dollyCamera.set_model(noModel);

    loader.add_game_object(gamer);
    loader.add_game_object(dollyCamera);
    loader.add_game_object(physObj);
    loader.add_game_object(targetStartingPoint);
    loader.add_game_object(targetEndingPoint);

    loader.add_camera(cameraFree);
    loader.add_camera(dollyCamera);
    loader.set_active_camera(&cameraFree);

    InputHandler::addKeyMapping(GLFW_KEY_TAB,
                                [&dollyCamera, &loader](Action action) {
                                    const glm::vec4 direction = Player::FRONT;
                                    if (action == GLFW_PRESS) {
                                        loader.set_active_camera(&dollyCamera);
                                        dollyCamera.set_progress(0.0f);
                                        dollyCamera.startMoving();
                                    }
                                });
    
    InputHandler::addKeyMapping(GLFW_KEY_CAPS_LOCK,
                            [&cameraFree, &loader](Action action) {
                                const glm::vec4 direction = Player::FRONT;
                                if (action == GLFW_PRESS) {
                                    loader.set_active_camera(&cameraFree);
                                }
                            });

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

    gamer.set_drag(0.6);
    auto window = loader.get_window();

    loader.start([&gamer, &dollyCamera, &physObj, &loader]() {
        const GLfloat deltaTime = Loader::get_delta_t();
        gamer.update(deltaTime);
        physObj.update(deltaTime);
        dollyCamera.update(deltaTime);
    });
    return 0;
}
