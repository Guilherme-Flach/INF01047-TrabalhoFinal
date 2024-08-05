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
    auto playerCollider = BoxCollider(&player, {0.0, 0.0, 0.0, 1.0}, 10, 5, 5);
    player.addChild(playerCollider);

    auto playerShipCollider =
        SphereCollider(&player.get_ship(), {0.0, 0.0, 0.0, 1.0}, 1.0f);
    player.get_ship().addChild(playerShipCollider);

    Camera cameraPanoramic =
        Camera({60.0f, 60.0f, 60.0f, 1.0f},
               new GameObject(GameObjectType::STANDARD, ORIGIN));
    cameraPanoramic.set_fov(0.6 * M_PI);
    cameraPanoramic.set_farPlane(-200.0f);

    const BezierPath_Quadratic cameraPathToPanoramic =
        QuadraticInterpolator::createPath(
            player.get_playerCamera().get_global_position(),
            cameraPanoramic.get_global_position());

    const BezierPath_Quadratic targetPathToPanoramic =
        QuadraticInterpolator::createPath(
            player.get_playerCamera().get_target()->get_global_position(),
            cameraPanoramic.get_target()->get_global_position());

    PhysicsObject physObj = PhysicsObject({0.0f, 0.0f, 0.0f, 1.0f}, 1.0f);

    const BezierPath_Quadratic lensPathToPanoramic = {
        {player.get_playerCamera().get_fov(),
         player.get_playerCamera().get_nearPlane(),
         player.get_playerCamera().get_farPlane(), 1.0f},
        {0.6 * M_PI, player.get_playerCamera().get_nearPlane(),
         player.get_playerCamera().get_farPlane(), 1.0f},
        {0.8 * M_PI, player.get_playerCamera().get_nearPlane(),
         player.get_playerCamera().get_farPlane(), 1.0f}};

    DollyCamera dollyCameraPlayerToPanoramic =
        DollyCamera(cameraPathToPanoramic, 0.7f, &physObj,
                    targetPathToPanoramic, 0.5f, lensPathToPanoramic);

    dollyCameraPlayerToPanoramic.set_onUpdate(
        [&dollyCameraPlayerToPanoramic, &loader,
         &cameraPanoramic](GLfloat deltaTime) -> void {
            if (dollyCameraPlayerToPanoramic.get_isFinished()) {
                loader.set_active_camera(&cameraPanoramic);
                dollyCameraPlayerToPanoramic.set_progress(0.0f);
            }
        });

    const BezierPath_Quadratic cameraPathToPlayer =
        QuadraticInterpolator::createPath(
            player.get_playerCamera().get_global_position(),
            cameraPanoramic.get_global_position());

    const BezierPath_Quadratic targetPathToPlayer =
        QuadraticInterpolator::createPath(
            player.get_playerCamera().get_target()->get_global_position(),
            cameraPanoramic.get_target()->get_global_position());

    const BezierPath_Quadratic lensPathToPlayer = {
        {cameraPanoramic.get_fov(), cameraPanoramic.get_nearPlane(),
         cameraPanoramic.get_farPlane(), 1.0f},
        {player.get_playerCamera().get_fov(),
         player.get_playerCamera().get_nearPlane(),
         player.get_playerCamera().get_farPlane(), 1.0f},
        {0.1 * M_PI, player.get_playerCamera().get_nearPlane(),
         player.get_playerCamera().get_farPlane(), 1.0f}};

    DollyCamera dollyCameraPanoramicToPlayer =
        DollyCamera(cameraPathToPanoramic, 0.7f, &physObj,
                    targetPathToPanoramic, 0.5f, lensPathToPlayer);

    dollyCameraPanoramicToPlayer.set_onUpdate(
        [&dollyCameraPanoramicToPlayer, &loader,
         &player](GLfloat deltaTime) -> void {
            if (dollyCameraPanoramicToPlayer.get_isFinished()) {
                loader.set_active_camera(&player.get_playerCamera());
                dollyCameraPanoramicToPlayer.set_progress(0.0f);
            }
        });

    loader.set_active_camera(&player.get_playerCamera());

    // Locking
    // InputHandler::addKeyMapping(GLFW_KEY_T, [&ship, &sun](Action action) {
    //     if (action == GLFW_PRESS) {
    //         ship.set_currentPlanet(&sun);
    //     }
    // });

    InputHandler::addKeyMapping(GLFW_KEY_TAB, [&dollyCameraPlayerToPanoramic,
                                               &loader, &player,
                                               &cameraPanoramic,
                                               &dollyCameraPanoramicToPlayer](
                                                  Action action) {
        if (action == GLFW_PRESS) {
            // Transition to panoramic
            if (loader.get_active_camera() == &player.get_playerCamera()) {
                loader.set_active_camera(&dollyCameraPlayerToPanoramic);

                const BezierPath_Quadratic cameraPath =
                    QuadraticInterpolator::createPath(
                        player.get_playerCamera().get_global_position(),
                        cameraPanoramic.get_global_position());
                const BezierPath_Quadratic targetPath =
                    QuadraticInterpolator::createPath(
                        player.get_playerCamera()
                            .get_target()
                            ->get_global_position(),
                        cameraPanoramic.get_target()->get_global_position());

                dollyCameraPlayerToPanoramic.set_cameraPath(cameraPath);
                dollyCameraPlayerToPanoramic.set_targetPath(targetPath);

                dollyCameraPlayerToPanoramic.set_progress(0.0f);
                dollyCameraPlayerToPanoramic.startMoving();

                // Transition to Ship
            } else if (loader.get_active_camera() == &cameraPanoramic) {
                loader.set_active_camera(&dollyCameraPanoramicToPlayer);

                const BezierPath_Quadratic cameraPath =
                    QuadraticInterpolator::createPath(
                        cameraPanoramic.get_global_position(),
                        player.get_playerCamera().get_global_position());
                const BezierPath_Quadratic targetPath =
                    QuadraticInterpolator::createPath(
                        cameraPanoramic.get_target()->get_global_position(),
                        player.get_playerCamera()
                            .get_target()
                            ->get_global_position());

                dollyCameraPanoramicToPlayer.set_cameraPath(cameraPath);
                dollyCameraPanoramicToPlayer.set_targetPath(targetPath);

                dollyCameraPanoramicToPlayer.set_progress(0.0f);
                dollyCameraPanoramicToPlayer.startMoving();
            }
        }
    });
    CollisionsManager manager;
    // manager.add_or_update_collider(playerCollider);
    manager.add_or_update_collider(playerShipCollider);
    auto planets = s.get_planets();
    for (std::vector<Planet *>::iterator node = planets.begin();
         node != planets.end(); node++) {
        manager.add_object(**node);
    }
    // Renderer::instance().addToRenderQueue(Renderer::RenderMode::GOURAUD,
    // &sun);
    loader.start([&]() {
        const GLfloat deltaTime = Loader::get_delta_t();
        dollyCameraPlayerToPanoramic.update(deltaTime);
        dollyCameraPanoramicToPlayer.update(deltaTime);
        player.get_playerCamera().update(deltaTime);
        s.FixedUpdate(deltaTime);
        manager.update_colliders();
        manager.handle_collisions(deltaTime);
    });
    return 0;
}
