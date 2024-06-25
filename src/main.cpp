#include "engine/loader.hpp"
#include "GLFW/glfw3.h"
#include <functional>
#include <iostream>

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";

    auto loader = Loader(width, height, title);

    addKeymap({GLFW_KEY_W, GLFW_PRESS},
              []() { std::cout << "GAMING" << std::endl; });

    loader.start();
    return 0;
}
