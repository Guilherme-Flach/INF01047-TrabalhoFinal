#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/camera/dollyCamera.hpp"
#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/player.hpp"
#include "engine/Rendering/defaultModels.hpp"
#include "engine/Rendering/model3D.hpp"
#include "engine/loader.hpp"
#include "engine/Input/inputHandler.hpp"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float4.hpp"
#include "engine/interpolator.hpp"
#include "matrices.hpp"
#include <functional>
#include <iostream>

static glm::vec4 ORIGIN = {0.0f, 0.0f, 0.0f, 1.0f};
static glm::vec4 FRONT = {0.0f, 0.0f, 1.0f, 1.0f};
static glm::vec4 UP = {0.0f, 1.0f, 0.0f, 1.0f};

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";

    auto loader = Loader(width, height, title);

    Model3D bunnyModel = Model3D("../../data/bunny.obj");
    Model3D ballModel = Model3D("../../data/sphere.obj");
    // Model3D wireCubeModel = WireCube();
    // Model3D dotModel = DotModel();
    // Model3D noModel = NoModel();
    // Model3D axesModel = BaseAxesModel();

    PhysicsObject sun = PhysicsObject(ORIGIN, 5000.0f);
    sun.set_model(ballModel);
    sun.set_modelScaling({5.0f, 5.0f, 5.0f});
    

    PhysicsObject sunBall = PhysicsObject({8.0f, 0.0f, 0.0f, 1.0f}, 20.0f);
    sunBall.set_model(ballModel);
    sunBall.set_modelScaling({3.0f, 3.0f, 3.0f});
    sun.addChild(sunBall);
    sunBall.applyForce({0.0f, 0.5f, 0.0f, 0.0f});


    Player gamer = Player({0.0f, 0.0f, -2.0f, 1.0f});

    FreeCamera* cameraPlayer = (FreeCamera*) gamer.get_camera();
    //cameraPlayer->set_model(noModel);
    //cameraPlayer->get_target()->set_model(dotModel);

    FreeCamera* ship = (FreeCamera*) gamer.get_ship();
    ship->set_model(bunnyModel);
    //ship->set_model(wireCubeModel);


    static glm::vec2 prevPos = InputHandler::getMousePos();
    cameraPlayer->set_onUpdate([&cameraPlayer](GLfloat deltaTime) -> void {
        glm::vec2 mousePos = InputHandler::getMousePos();
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = mousePos.x - prevPos.x;
        float dy = mousePos.y - prevPos.y;

        // Atualizamos parâmetros da câmera com os deslocamentos

        cameraPlayer->rotate(-0.01f * dx, cameraPlayer->get_v_vector());
        cameraPlayer->rotate(-0.01f * dy, cameraPlayer->get_u_vector());

        // UGLY desfaz rotação se for zoado
        const float w_up_dotProduct = dotproduct(cameraPlayer->get_w_vector(),
                                                 cameraPlayer->get_up_vector());
        if (w_up_dotProduct >= FreeCamera::heightLimit ||
            w_up_dotProduct <= -FreeCamera::heightLimit) {
            cameraPlayer->rotate(0.01f * dy, cameraPlayer->get_u_vector());
        }

        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        prevPos.x = mousePos.x;
        prevPos.y = mousePos.y;
    });

    Camera cameraPanoramic =
        Camera({10.0f, 10.0f, 10.0f, 1.0f}, new GameObject(ORIGIN));
    //cameraPanoramic.set_model(noModel);
    cameraPanoramic.set_fov(0.6 * M_PI);
    cameraPanoramic.set_farPlane(-500.0f);



    const BezierPath_Quadratic cameraPathToPanoramic =
        QuadraticInterpolator::createPath(
            cameraPlayer->get_global_position(),
            cameraPanoramic.get_global_position());

    const BezierPath_Quadratic targetPathToPanoramic =
        QuadraticInterpolator::createPath(
            cameraPlayer->get_target()->get_global_position(),
            cameraPanoramic.get_target()->get_global_position());

    PhysicsObject physObj = PhysicsObject({0.0f, 0.0f, 0.0f, 1.0f}, 1.0f);
    //physObj.set_model(cuboModel);

    const BezierPath_Quadratic lensPathToPanoramic = {
        {cameraPlayer->get_fov(), cameraPlayer->get_nearPlane(),
         cameraPlayer->get_farPlane(), 1.0f},
        {0.6 * M_PI, cameraPlayer->get_nearPlane(), -50.0f, 1.0f},
        {0.8 * M_PI, cameraPlayer->get_nearPlane(), -50.0f, 1.0f}};

    DollyCamera dollyCameraPlayerToPanoramic =
        DollyCamera(cameraPathToPanoramic, 0.7f, &physObj,
                    targetPathToPanoramic, 0.5f, lensPathToPanoramic);

    //dollyCameraPlayerToPanoramic.set_model(noModel);

    dollyCameraPlayerToPanoramic.set_onUpdate(
        [&dollyCameraPlayerToPanoramic, &loader,
         &cameraPanoramic](GLfloat deltaTime) -> void {
            if (dollyCameraPlayerToPanoramic.get_isFinished()) {
                loader.set_active_camera(&cameraPanoramic);
                dollyCameraPlayerToPanoramic.set_progress(0.0f);
            }
        });

    const BezierPath_Quadratic cameraPathToPlayer =
        QuadraticInterpolator::createPath(
            cameraPlayer->get_global_position(),
            cameraPanoramic.get_global_position());

    const BezierPath_Quadratic targetPathToPlayer =
        QuadraticInterpolator::createPath(
            cameraPlayer->get_target()->get_global_position(),
            cameraPanoramic.get_target()->get_global_position());

    const BezierPath_Quadratic lensPathToPlayer = {
        {cameraPanoramic.get_fov(), cameraPanoramic.get_nearPlane(),
         cameraPanoramic.get_farPlane(), 1.0f},
        {cameraPlayer->get_fov(), cameraPlayer->get_nearPlane(),
         cameraPlayer->get_farPlane(), 1.0f},
        {0.1 * M_PI, cameraPlayer->get_nearPlane(), cameraPlayer->get_farPlane(),
         1.0f}};

    DollyCamera dollyCameraPanoramicToPlayer =
        DollyCamera(cameraPathToPanoramic, 0.7f, &physObj,
                    targetPathToPanoramic, 0.5f, lensPathToPlayer);

    //dollyCameraPanoramicToPlayer.set_model(noModel);

    dollyCameraPanoramicToPlayer.set_onUpdate(
        [&dollyCameraPanoramicToPlayer, &loader,
         &cameraPlayer](GLfloat deltaTime) -> void {
            if (dollyCameraPanoramicToPlayer.get_isFinished()) {
                loader.set_active_camera(cameraPlayer);
                dollyCameraPanoramicToPlayer.set_progress(0.0f);
            }
        });

    loader.add_game_object(gamer);
    loader.add_game_object(sun);

    loader.add_camera(*cameraPlayer);
    loader.add_camera(dollyCameraPlayerToPanoramic);
    loader.add_camera(dollyCameraPanoramicToPlayer);
    loader.set_active_camera(cameraPlayer);

    // Locking
    // InputHandler::addKeyMapping(GLFW_KEY_T, [&gamer, &sun](Action action) {
    //     if (action == GLFW_PRESS) {
    //         sun.addChild(gamer);
    //         std::cout << "balls"<< std::endl;
    //     }
    // });

    InputHandler::addKeyMapping(GLFW_KEY_TAB, [&dollyCameraPlayerToPanoramic,
                                               &loader, &cameraPlayer,
                                               &cameraPanoramic,
                                               &dollyCameraPanoramicToPlayer](
                                                  Action action) {
        if (action == GLFW_PRESS) {
            // Transition to panoramic
            if (loader.get_active_camera() == cameraPlayer) {
                loader.set_active_camera(&dollyCameraPlayerToPanoramic);

                const BezierPath_Quadratic cameraPath =
                    QuadraticInterpolator::createPath(
                        cameraPlayer->get_global_position(),
                        cameraPanoramic.get_global_position());
                const BezierPath_Quadratic targetPath =
                    QuadraticInterpolator::createPath(
                        cameraPlayer->get_target()->get_global_position(),
                        cameraPanoramic.get_target()->get_global_position());

                dollyCameraPlayerToPanoramic.set_cameraPath(cameraPath);
                dollyCameraPlayerToPanoramic.set_targetPath(targetPath);

                dollyCameraPlayerToPanoramic.set_progress(0.0f);
                dollyCameraPlayerToPanoramic.startMoving();

                // Transition to player
            } else if (loader.get_active_camera() == &cameraPanoramic) {
                loader.set_active_camera(&dollyCameraPanoramicToPlayer);

                const BezierPath_Quadratic cameraPath =
                    QuadraticInterpolator::createPath(
                        cameraPanoramic.get_global_position(),
                        cameraPlayer->get_global_position());
                const BezierPath_Quadratic targetPath =
                    QuadraticInterpolator::createPath(
                        cameraPanoramic.get_target()->get_global_position(),
                        cameraPlayer->get_target()->get_global_position());

                dollyCameraPanoramicToPlayer.set_cameraPath(cameraPath);
                dollyCameraPanoramicToPlayer.set_targetPath(targetPath);

                dollyCameraPanoramicToPlayer.set_progress(0.0f);
                dollyCameraPanoramicToPlayer.startMoving();
            }
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
    [&gamer, &cameraPlayer](Action action) {
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
                                [&gamer, &cameraPlayer](Action action) {
                                    const glm::vec4 direction = Player::RIGHT;
                                    if (action == GLFW_PRESS) {
                                        gamer.increaseMovement(direction);
                                    } else if (action == GLFW_RELEASE) {
                                        gamer.increaseMovement(-direction);
                                    }
                                });

    InputHandler::addKeyMapping(GLFW_KEY_SPACE,
                                [&gamer, &cameraPlayer](Action action) {
                                    const glm::vec4 direction = Player::UP;
                                    if (action == GLFW_PRESS) {
                                        gamer.increaseMovement(direction);
                                    } else if (action == GLFW_RELEASE) {
                                        gamer.increaseMovement(-direction);
                                    }
                                });

    InputHandler::addKeyMapping(GLFW_KEY_LEFT_CONTROL,
                                [&gamer, &cameraPlayer](Action action) {
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

    InputHandler::addKeyMapping(GLFW_KEY_LEFT_SHIFT, [&gamer](Action action) {
        if (action == GLFW_PRESS) {
            gamer.set_isBoosting(true);
        } else if (action == GLFW_RELEASE) {
            gamer.set_isBoosting(false);
        }
    });

    gamer.set_drag(0.2f);
    auto window = loader.get_window();

    loader.start([&gamer, &dollyCameraPlayerToPanoramic, &physObj, &loader,
                  &cameraPlayer, &dollyCameraPanoramicToPlayer, &sun, &sunBall]() {
        const GLfloat deltaTime = Loader::get_delta_t();
        dollyCameraPlayerToPanoramic.update(deltaTime);
        dollyCameraPanoramicToPlayer.update(deltaTime);
        cameraPlayer->update(deltaTime);
        gamer.update(deltaTime);
        physObj.update(deltaTime);
        sun.update(deltaTime);
        sun.rotate(0.4*deltaTime, UP);
        //sunBall.update(deltaTime);
    });
    return 0;
}
