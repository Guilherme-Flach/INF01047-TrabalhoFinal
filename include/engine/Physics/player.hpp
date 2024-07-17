#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/ship.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"
#include "ship.hpp"

class Player : public PhysicsObject {
  private:
    const static GLfloat speedLimit;
    const static GLfloat playerSpeed;
    const static GLfloat playerMass;

    const static glm::vec4 startingPosition;

  protected:
    FreeCamera playerCamera = FreeCamera({0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f});
    Ship ship = Ship(startingPosition);

    glm::vec3 playerMovement = {0.0f, 0.0f, 0.0f};
    

  public:
    Player();

    FreeCamera& get_playerCamera() { return playerCamera; }
    Ship& get_ship() { return ship; }

};

#endif // PLAYER_HEADER
