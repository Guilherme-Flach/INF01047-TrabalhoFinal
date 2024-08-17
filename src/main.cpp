#include "engine/Physics/player.hpp"
#include "engine/Physics/solarSystem.hpp"
#include "engine/Rendering/defaultModels.hpp"
#include "engine/Rendering/model3D.hpp"
#include "engine/loader.hpp"
#include <csetjmp>
#include <functional>

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";

    auto loader = Loader(width, height, title);

    SolarSystem s = SolarSystem();

    Player &player = s.get_player();

    loader.set_active_camera(&player.get_playerCamera());

    loader.start([&]() {
        const GLfloat deltaTime = Loader::get_delta_t();
        player.update(deltaTime);
        s.FixedUpdate(deltaTime);
    });
    return 0;
}
