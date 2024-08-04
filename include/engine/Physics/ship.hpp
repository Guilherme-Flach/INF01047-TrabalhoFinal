#ifndef SHIP_HEADER
#define SHIP_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "collider.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"

class Ship : public PhysicsObject {
  private:
    constexpr static const GLfloat speedLimit = 20.0f;
    constexpr static const GLfloat playerMass = 1.00f;
    static const GLfloat shipDrag;
    static const GLfloat shipBaseSpeed;
    static const GLfloat shipBoostingSpeed;

  protected:
    GameObject shipContainer;
    SphereCollider shipCollider;
    glm::vec4 thrusters = {0.0f, 0.0f, 0.0f, 0.0f};
    glm::vec4 rotationRate = {0.0f, 0.0f, 0.0f, 0.0f};
    bool isBoosting;

  public:
    Ship(glm::vec4 position);

    static const GLfloat turningSpeed;

    constexpr static const glm::vec4 ROTATE_FRONT = {+1.0f, 0.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_BACK = {-1.0f, 0.0f, 0.0f, 0.0f};

    constexpr static const glm::vec4 ROTATE_RIGHT = {0.0f, -1.0f, 0.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_LEFT = {0.0f, +1.0f, 0.0f, 0.0f};

    constexpr static const glm::vec4 ROTATE_CLKWISE = {0.0f, 0.0f, +1.0f, 0.0f};
    constexpr static const glm::vec4 ROTATE_CCLKWISE = {0.0f, 0.0f, -1.0f,
                                                        0.0f};

    void physicsUpdate(GLfloat deltaTime);
    void powerThrusters(glm::vec4 thrusters) {
        set_thrusting(this->thrusters + thrusters);
    };
    void increaseRotationRate(glm::vec4 rate) {
        set_rotatioRate(this->rotationRate + rate);
    };

    SphereCollider &get_shipCollider() { return shipCollider; };
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
