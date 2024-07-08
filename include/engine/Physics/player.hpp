#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"

class Player : public PhysicsObject {
  private:
    constexpr static const GLfloat speedLimit = 20.0f;
    constexpr static const GLfloat playerMass = 2.0f;
    constexpr static const GLfloat playerDrag = 0.9f;
    static GLfloat playerBaseSpeed;
    static GLfloat playerBoostSpeed;
    static GLfloat cameraRotationSpeed;

  protected:
    Camera *camera = nullptr;
    Camera *ship = nullptr;
    glm::vec4 movement;
    glm::vec4 rotationRate;
    bool isBoosting;
    bool isPiloting;

  public:
    constexpr static const glm::vec4 UP = {0.0f, 1.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 DOWN = {0.0f, -1.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 LEFT = {-1.0f, 0.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 RIGHT = {1.0f, 0.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 FRONT = {0.0f, 0.0f, -1.0f, 0.0f};
    constexpr static const glm::vec4 BACK = {0.0f, 0.0f, 1.0f, 0.0f};

    constexpr static const glm::vec4 ROTATE_FRONT = {-1.0f, 0.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_BACK = {1.0f, 0.0f, 0.0f, 0.0f};

    constexpr static const glm::vec4 ROTATE_RIGHT = {0.0f, -1.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_LEFT = {0.0f, 1.0f, 0.0f, 0.0f};

    constexpr static const glm::vec4 ROTATE_CLKWISE = {0.0f, 0.0f, -1.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_CCLKWISE = {0.0f, 0.0f, 1.0f, 0.0f};

    Player(glm::vec4 position)
        : PhysicsObject(position, playerMass),
          camera(new FreeCamera({0.0f, 0.0f, 0.0f, 1.0f},
                                {0.0f, 0.0f, 1.0f, 1.0f})),
          ship(new FreeCamera({0.0f, 0.0f, 0.0f, 1.0f},
                                {0.0f, 0.0f, 1.0f, 1.0f})),
          movement({0.0f, 0.0f, 0.0f, 0.0f}),
          rotationRate({0.0f, 0.0f, 0.0f, 0.0f}) {
        drag = playerDrag;
        this->addChild(*camera);
        this->addChild(*ship);
    };
    void update(GLfloat deltaTime);
    void increaseMovement(glm::vec4 movement) {
        set_movement(this->movement + movement);
    };
    void increaseRotationRate(glm::vec4 rate) {
        set_rotatioRate(this->rotationRate + rate);
    };

    Camera *get_camera() { return camera; }
    Camera *get_ship() { return ship; }
    glm::vec4 get_movement() { return movement; }
    glm::vec4 get_rotatioRate() { return rotationRate; }

    void set_camera(Camera *camera) { this->camera = camera; }
    void set_ship(Camera *ship) { this->ship = ship; }
    void set_movement(glm::vec4 movement) { this->movement = movement; }
    void set_rotatioRate(glm::vec4 rotationRate) {
        this->rotationRate = rotationRate;
    }
    void set_isBoosting(bool isBoosting) { this->isBoosting = isBoosting; }
};

#endif // PLAYER_HEADER
