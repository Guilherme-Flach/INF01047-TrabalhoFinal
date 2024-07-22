#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "collider.hpp"
#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/ship.hpp"
#include "engine/Rendering/model3D.hpp"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include "ship.hpp"

class Player : public PhysicsObject {
  private:
    const static GLfloat speedLimit;
    const static GLfloat playerSpeed;
    const static GLfloat playerMass;

    const static glm::vec4 startingPosition;

  protected:
    FreeCamera playerCamera = FreeCamera(ORIGIN, FRONT);
    Ship ship = Ship(startingPosition);
    SphereCollider shipCheck = SphereCollider(ORIGIN,  1.0f);
    BoxCollider playerCollider = BoxCollider(ORIGIN, 0.5f, 1.0f, 0.5f);

    glm::vec3 playerMovement = {0.0f, 0.0f, 0.0f};

    bool isPiloting;    

  public:
    Player();

    FreeCamera& get_playerCamera() { return playerCamera; }
    Ship& get_ship() { return ship; }
    BoxCollider& get_playerCollider() { return playerCollider; }

    void physicsUpdate(GLfloat deltaTime);

};

#endif // PLAYER_HEADER
