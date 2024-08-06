#include "engine/Physics/player.hpp"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/Input/inputHandler.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "matrices.hpp"

const GLfloat Player::speedLimit = 10.0f;
const GLfloat Player::playerSpeed = 2.0f;
const GLfloat Player::playerMass = 1.0f;

const glm::vec4 Player::startingPosition = {49.0f, 49.0f, 49.0f, 1.0f};

Player::Player()
    : PhysicsObject(ORIGIN, playerMass),
      playerCamera(FreeCamera(ORIGIN, FRONT)), ship(Ship(startingPosition)),
      playerMovement({0.0f, 0.0f, 0.0f}), isPiloting(true) {
    addChild(playerCamera);
    set_parent(ship);

    playerCamera.rotate(M_PI / 2.0f, UP);

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
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = mousePos.x - prevPos.x;
        float dy = mousePos.y - prevPos.y;

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
            if (isPiloting) {
                isPiloting = false;
                set_position(get_global_position());
                remove_parent();
            } else {
                isPiloting = true;
                set_position({0.0f, 0.0f, 0.0f, 1.0f});
                acceleration = {0.0f, 0.0f, 0.0f, 0.0f};
                set_parent(ship);
            }
        }
    });
}

void Player::physicsUpdate(GLfloat deltaTime) {
    if (isPiloting) {
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
