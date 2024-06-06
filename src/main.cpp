#include "engine/loader.hpp"
#include "GLFW/glfw3.h"
#include <functional>
#include <iostream>

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";
    auto loader = Loader(width, height, title);
    addKeyMap({GLFW_KEY_W, GLFW_PRESS},
              []() { std::cout << "BUNDA ASSADA LO0SER" << std::endl; });
    glfwSetKeyCallback(loader.get_window(), handleKeymaps);
    loader.start();
    return 0;
}
