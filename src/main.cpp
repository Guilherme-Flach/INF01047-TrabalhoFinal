#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/camera/dollyCamera.hpp"
#include "engine/EngineObject/camera/freeCamera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Physics/planet.hpp"
#include "engine/Physics/player.hpp"
#include "engine/Physics/solarSystem.hpp"
#include "engine/Rendering/defaultModels.hpp"
#include "engine/Rendering/model3D.hpp"
#include "engine/loader.hpp"
#include "engine/Input/inputHandler.hpp"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float4.hpp"
#include "engine/interpolator.hpp"
#include <csetjmp>
#include <functional>
#include "engine/Physics/collider.hpp"
#include "matrices.hpp"

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";

    auto loader = Loader(width, height, title);

    SolarSystem s = SolarSystem();

    Player &player = s.get_player();
    Camera &cameraPanoramic = player.get_panoramicCamera();

    loader.set_active_camera(&player.get_playerCamera());

    // Locking
    // InputHandler::addKeyMapping(GLFW_KEY_T, [&ship, &sun](Action action) {
    //     if (action == GLFW_PRESS) {
    //         ship.set_currentPlanet(&sun);
    //     }
    // });
    loader.start([&]() {
        const GLfloat deltaTime = Loader::get_delta_t();
        player.update(deltaTime);
        s.FixedUpdate(deltaTime);
    });
    return 0;
}
