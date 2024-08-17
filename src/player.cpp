#include "engine/Physics/player.hpp"
#include "GLFW/glfw3.h"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Input/inputHandler.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"
#include "engine/loader.hpp"
#include <iostream>
#include "engine/loader.hpp"

const GLfloat Player::speedLimit = 10.0f;
const GLfloat Player::playerSpeed = 2.0f;
const GLfloat Player::playerMass = 1.0f;

const glm::vec4 Player::startingPosition = {49.0f, 49.0f, 49.0f, 1.0f};
const glm::vec4 Player::panoramicCameraPosition = {0.01f, 50.0f, 0.01f, 1.0f};

const glm::vec4 POSITION_INSIDE_SHIP = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

const GLfloat transitionDurationCameraIn = 0.10f;
const GLfloat transitionDurationTargetIn = 0.20f;

const GLfloat transitionDurationCameraOut = 0.6f;
const GLfloat transitionDurationTargetOut = 0.2f;

GameObject g_OriginObject = GameObject(GameObjectType::STANDARD, ORIGIN);
GameObject g_DollyObject = GameObject(GameObjectType::STANDARD, ORIGIN);

Player::Player()
    : PhysicsObject(ORIGIN, playerMass),
      playerCamera(FreeCamera(POSITION_INSIDE_SHIP, FRONT)),
      panoramicCamera(Camera(panoramicCameraPosition, &g_OriginObject)),
      panoramicToPlayerDolly(
          QuadraticInterpolator::createPath(
              playerCamera.get_global_position(),
              panoramicCamera.get_global_position()),
          transitionDurationCameraOut, &g_DollyObject,
          QuadraticInterpolator::createPath(
              playerCamera.get_target()->get_global_position(),
              panoramicCamera.get_target()->get_global_position()),
          transitionDurationTargetOut,
          {{panoramicCamera.get_fov(), panoramicCamera.get_nearPlane(),
            panoramicCamera.get_farPlane(), 1.0f},
           {playerCamera.get_fov(), playerCamera.get_nearPlane(),
            playerCamera.get_farPlane(), 1.0f},
           {0.1 * M_PI, playerCamera.get_nearPlane(),
            playerCamera.get_farPlane(), 1.0f}}),
      playerToPanoramicDolly(
          QuadraticInterpolator::createPath(
              playerCamera.get_global_position(),
              panoramicCamera.get_global_position()),
          transitionDurationCameraIn, &g_DollyObject,
          QuadraticInterpolator::createPath(
              playerCamera.get_target()->get_global_position(),
              panoramicCamera.get_target()->get_global_position()),
          transitionDurationTargetIn,
          {{playerCamera.get_fov(), playerCamera.get_nearPlane(),
            playerCamera.get_farPlane(), 1.0f},
           {0.6 * M_PI, playerCamera.get_nearPlane(),
            playerCamera.get_farPlane(), 1.0f},
           {0.8 * M_PI, playerCamera.get_nearPlane(),
            playerCamera.get_farPlane(), 1.0f}}),
      ship(Ship(startingPosition)),
      // Maldade a frente, essas inicializações são só pra ter algo nas dolly
      // cameras
      playerMovement({0.0f, 0.0f, 0.0f}), isPiloting(true) {
    set_controlMode(Player::PILOTING_SHIP);
    addChild(playerCamera);
    set_parent(ship);
    panoramicCamera.set_fov(0.6 * M_PI);
    panoramicCamera.set_farPlane(-200.0f);

    playerToPanoramicDolly.set_onUpdate([this](GLfloat deltaTime) -> void {
        if (playerToPanoramicDolly.get_isFinished()) {
            Loader::set_active_camera(&panoramicCamera);
            glfwSetInputMode(Loader::get_window(), GLFW_CURSOR,
                             GLFW_CURSOR_NORMAL);

            int width, height;
            glfwGetFramebufferSize(Loader::get_window(), &width, &height);
            glfwSetCursorPos(Loader::get_window(), width / 2.0f, height / 2.0f);

            set_controlMode(PANORAMIC);
            playerToPanoramicDolly.set_progress(0.0f);
        }
    });

    panoramicToPlayerDolly.set_onUpdate([this](GLfloat deltaTime) -> void {
        if (panoramicToPlayerDolly.get_isFinished()) {
            Loader::set_active_camera(&playerCamera);
            set_controlMode(PILOTING_SHIP);
            panoramicToPlayerDolly.set_progress(0.0f);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_TAB, [this](Action action) {
        if (action == GLFW_PRESS) {
            // Transition to panoramic
            if (controlMode == PILOTING_SHIP) {
                Loader::set_active_camera(&playerToPanoramicDolly);
                controlMode = TRANSITIONING;

                const BezierPath_Quadratic cameraPath =
                    QuadraticInterpolator::createPath(
                        playerCamera.get_global_position(),
                        panoramicCamera.get_global_position());
                const BezierPath_Quadratic targetPath =
                    QuadraticInterpolator::createPath(
                        playerCamera.get_target()->get_global_position(),
                        panoramicCamera.get_target()->get_global_position());

                playerToPanoramicDolly.set_cameraPath(cameraPath);
                playerToPanoramicDolly.set_targetPath(targetPath);

                playerToPanoramicDolly.set_progress(0.0f);
                playerToPanoramicDolly.startMoving();

                // Transition to Ship
            } else if (controlMode == PANORAMIC) {
                Loader::set_active_camera(&panoramicToPlayerDolly);
                controlMode = TRANSITIONING;
                glfwSetInputMode(Loader::get_window(), GLFW_CURSOR,
                                 GLFW_CURSOR_DISABLED);

                const BezierPath_Quadratic cameraPath =
                    QuadraticInterpolator::createPath(
                        panoramicCamera.get_global_position(),
                        playerCamera.get_global_position());
                const BezierPath_Quadratic targetPath =
                    QuadraticInterpolator::createPath(
                        panoramicCamera.get_target()->get_global_position(),
                        playerCamera.get_target()->get_global_position());

                panoramicToPlayerDolly.set_cameraPath(cameraPath);
                panoramicToPlayerDolly.set_targetPath(targetPath);

                panoramicToPlayerDolly.set_progress(0.0f);
                panoramicToPlayerDolly.startMoving();
            }
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_W, [this](Action action) {
        const glm::vec4 direction = FRONT;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_S, [this](Action action) {
        const glm::vec4 direction = BACK;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_A, [this](Action action) {
        const glm::vec4 direction = LEFT;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_D, [this](Action action) {
        const glm::vec4 direction = RIGHT;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_SPACE, [this](Action action) {
        const glm::vec4 direction = UP;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_LEFT_CONTROL, [this](Action action) {
        const glm::vec4 direction = DOWN;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_UP, [this](Action action) {
        const glm::vec4 rotation = Ship::ROTATE_BACK;
        if (action == GLFW_PRESS) {
            ship.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            ship.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_DOWN, [this](Action action) {
        const glm::vec4 rotation = Ship::ROTATE_FRONT;
        if (action == GLFW_PRESS) {
            ship.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            ship.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_LEFT, [this](Action action) {
        const glm::vec4 rotation = Ship::ROTATE_LEFT;
        if (action == GLFW_PRESS) {
            ship.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            ship.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_RIGHT, [this](Action action) {
        const glm::vec4 rotation = Ship::ROTATE_RIGHT;
        if (action == GLFW_PRESS) {
            ship.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            ship.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_Q, [this](Action action) {
        const glm::vec4 rotation = Ship::ROTATE_CCLKWISE;
        if (action == GLFW_PRESS) {
            ship.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            ship.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_E, [this](Action action) {
        const glm::vec4 rotation = Ship::ROTATE_CLKWISE;
        if (action == GLFW_PRESS) {
            ship.increaseRotationRate(rotation);
        } else if (action == GLFW_RELEASE) {
            ship.increaseRotationRate(-rotation);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_LEFT_SHIFT, [this](Action action) {
        if (action == GLFW_PRESS) {
            ship.set_isBoosting(true);
        } else if (action == GLFW_RELEASE) {
            ship.set_isBoosting(false);
        }
    });

    static glm::vec2 prevPos = InputHandler::getMousePos();
    playerCamera.set_onUpdate([this](GLfloat deltaTime) -> void {
        glm::vec2 mousePos = InputHandler::getMousePos();
        float dx = mousePos.x - prevPos.x;
        float dy = mousePos.y - prevPos.y;

        if (controlMode != PILOTING_SHIP) {
            prevPos.x = mousePos.x;
            prevPos.y = mousePos.y;
            return;
        }
        playerCamera.rotate(-0.01f * dx, playerCamera.get_v_vector());
        playerCamera.rotate(-0.01f * dy, playerCamera.get_u_vector());

        const float w_up_dotProduct = dotproduct(playerCamera.get_w_vector(),
                                                 playerCamera.get_up_vector());
        if (w_up_dotProduct >= FreeCamera::heightLimit ||
            w_up_dotProduct <= -FreeCamera::heightLimit) {
            playerCamera.rotate(0.01f * dy, playerCamera.get_u_vector());
        }

        prevPos.x = mousePos.x;
        prevPos.y = mousePos.y;
    });

    // Board Ship
    InputHandler::addKeyMapping(GLFW_KEY_T, [this](Action action) {
        if (action == GLFW_PRESS) {
            if (controlMode == PILOTING_SHIP) {
                controlMode = FREE;
                set_position(get_global_position());
                remove_parent();
            } else {
                controlMode = PILOTING_SHIP;
                set_position(ORIGIN);
                acceleration = {0.0f, 0.0f, 0.0f, 0.0f};
                set_parent(ship);
            }
        }
    });
}

void Player::update(GLfloat deltaTime) {

    playerCamera.update(deltaTime);
    panoramicToPlayerDolly.update(deltaTime);
    playerToPanoramicDolly.update(deltaTime);
    GameObject::update(deltaTime);
}

void Player::physicsUpdate(GLfloat deltaTime) {
    if (controlMode == PILOTING_SHIP) {
        // Update camera to follow ship
        const glm::mat3x4 shipBasis =
            ship.get_shipContainer().get_global_basis();

        playerCamera.rotate(ship.get_rotatioRate().x * deltaTime *
                                Ship::turningSpeed,
                            shipBasis[0]);

        playerCamera.rotate(ship.get_rotatioRate().y * deltaTime *
                                Ship::turningSpeed,
                            shipBasis[1]);

        playerCamera.rotate(ship.get_rotatioRate().z * deltaTime *
                                Ship::turningSpeed,
                            shipBasis[2]);
        playerCamera.set_up_vector(shipBasis[1]);
    } else {
        // if (currentPlanet != nullptr) {
        //     const glm::vec4 out_vector = this->get_global_position() -
        //                                  currentPlanet->get_global_position();
        //     const float distance = norm(out_vector);
        //     playerCamera->set_up_vector(out_vector / distance);
        //     set_up_vector(out_vector / distance);
        // }
        PhysicsObject::physicsUpdate(deltaTime);
    }
}

void Player::set_controlMode(ControlMode controlMode) {
    if (controlMode == PILOTING_SHIP) {
        Loader::set_globalState(Loader::StateFlag::VIEW_TYPE,
                                Loader::StateValue::VIEW_SHIP);
    } else if (controlMode == PANORAMIC) {
        Loader::set_globalState(Loader::StateFlag::VIEW_TYPE,
                                Loader::StateValue::VIEW_PANORAMIC);
    }

    this->controlMode = controlMode;
}
