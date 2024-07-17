#include "engine/Physics/player.hpp"
#include "engine/Input/inputHandler.hpp"
#include "matrices.hpp"

const GLfloat Player::speedLimit = 10.0f;
const GLfloat Player::playerSpeed = 2.0f;
const GLfloat Player::playerMass = 0.5f;

const glm::vec4 Player::startingPosition = {-5.0f, -5.0f, -5.0f, 1.0f};

Player::Player() : PhysicsObject(startingPosition, playerMass) {
    addChild(playerCamera);

    InputHandler::addKeyMapping(GLFW_KEY_W, [this](Action action) {
        const glm::vec4 direction = Ship::FRONT;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_S, [this](Action action) {
        const glm::vec4 direction = Ship::BACK;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_A, [this](Action action) {
        const glm::vec4 direction = Ship::LEFT;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_D, [this](Action action) {
        const glm::vec4 direction = Ship::RIGHT;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_SPACE, [this](Action action) {
        const glm::vec4 direction = Ship::UP;
        if (action == GLFW_PRESS) {
            ship.powerThrusters(direction);
        } else if (action == GLFW_RELEASE) {
            ship.powerThrusters(-direction);
        }
    });

    InputHandler::addKeyMapping(GLFW_KEY_LEFT_CONTROL, [this](Action action) {
        const glm::vec4 direction = Ship::DOWN;
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

        // Atualizamos parâmetros da câmera com os deslocamentos

        playerCamera.rotate(-0.01f * dx, playerCamera.get_v_vector());
        playerCamera.rotate(-0.01f * dy, playerCamera.get_u_vector());

        const float w_up_dotProduct = dotproduct(playerCamera.get_w_vector(),
                                                 playerCamera.get_up_vector());
        if (w_up_dotProduct >= FreeCamera::heightLimit ||
            w_up_dotProduct <= -FreeCamera::heightLimit) {
            playerCamera.rotate(0.01f * dy, playerCamera.get_u_vector());
        }

        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        prevPos.x = mousePos.x;
        prevPos.y = mousePos.y;
    });

}

void Player::physicsUpdate(GLfloat deltaTime) {
    ship.physicsUpdate(deltaTime);    
}