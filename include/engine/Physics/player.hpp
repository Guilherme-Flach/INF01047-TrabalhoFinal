#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "collider.hpp"
#include "engine/EngineObject/camera/dollyCamera.hpp"
#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/ship.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"
#include "ship.hpp"

class Player : public PhysicsObject {
  public:
    enum ControlMode { PILOTING_SHIP, FREE, PANORAMIC, TRANSITIONING };

  private:
    const static GLfloat speedLimit;
    const static GLfloat playerSpeed;
    const static GLfloat playerMass;

    const static glm::vec4 startingPosition;
    const static glm::vec4 panoramicCameraPosition;

  protected:
    FreeCamera playerCamera;
    Camera panoramicCamera;

    DollyCamera panoramicToPlayerDolly;
    DollyCamera playerToPanoramicDolly;

    Ship ship;

    glm::vec3 playerMovement;
    ControlMode controlMode;

    bool isPiloting;

  public:
    Player();

    ControlMode get_controlMode() { return controlMode; }
    FreeCamera &get_playerCamera() { return playerCamera; }
    Camera &get_panoramicCamera() { return panoramicCamera; }
    Ship &get_ship() { return ship; }

    void set_controlMode(ControlMode ControlMode);

    void update(GLfloat deltaTime);
    void physicsUpdate(GLfloat deltaTime);
};

#endif // PLAYER_HEADER
