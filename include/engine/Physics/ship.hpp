#ifndef SHIP_HEADER
#define SHIP_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"
#include "planet.hpp"

class Ship : public PhysicsObject {
  private:
    constexpr static const GLfloat speedLimit = 20.0f;
    constexpr static const GLfloat playerMass = 2.0f;
    static const GLfloat shipDrag;
    static const GLfloat shipBaseSpeed;
    static const GLfloat shipBoostingSpeed;

    GameObject shipContainer = GameObject({0.0f, 0.0f, 0.0f, 1.0f});

  protected:

    glm::vec4 thrusters = {0.0f, 0.0f, 0.0f, 0.0f};
    glm::vec4 rotationRate = {0.0f, 0.0f, 0.0f, 0.0f};
    bool isBoosting;

  public:
    static const GLfloat turningSpeed;

    constexpr static const glm::vec4 ROTATE_FRONT = {+1.0f, 0.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_BACK  = {-1.0f, 0.0f, 0.0f, 0.0f};

    constexpr static const glm::vec4 ROTATE_RIGHT = {0.0f, -1.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_LEFT  = {0.0f, +1.0f, 0.0f, 0.0f};

    constexpr static const glm::vec4 ROTATE_CLKWISE  = {0.0f, 0.0f, +1.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_CCLKWISE = {0.0f, 0.0f, -1.0f, 0.0f};

    Ship(glm::vec4 position);
    void physicsUpdate(GLfloat deltaTime);
    void powerThrusters(glm::vec4 thrusters) {
        set_thrusting(this->thrusters + thrusters);
    };
    void increaseRotationRate(glm::vec4 rate) {
        set_rotatioRate(this->rotationRate + rate);
    };

    glm::vec4 get_thrusting() { return thrusters; }
    glm::vec4 get_rotatioRate() { return rotationRate; }
    GameObject &get_shipContainer() { return shipContainer; }

    void set_thrusting(glm::vec4 thrusting) { this->thrusters = thrusting; }
    void set_rotatioRate(glm::vec4 rotationRate) {
        this->rotationRate = rotationRate;
    }
    void set_isBoosting(bool isBoosting) { this->isBoosting = isBoosting; }
};

#endif // SHIP_HEADER
