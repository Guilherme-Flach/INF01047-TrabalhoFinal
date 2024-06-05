#include "engine/loader.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

std::map<KeyAction, std::function<void(void)>> keymaps;

void handleKeymaps(GLFWwindow *window, int key, int scan_code, int action,
                   int mod);

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";
    auto loader = Loader(width, height, title);
    keymaps[{GLFW_KEY_W, GLFW_PRESS}] = []() {
        std::cout << "BUNDA ASSADA LO0SER" << std::endl;
    };
    glfwSetKeyCallback(loader.get_window(), handleKeymaps);
    loader.start();
    return 0;
}

void handleKeymaps(GLFWwindow *window, int key, int scan_code, int action,
                   int mod) {
    KeyAction key_action;
    key_action.key = key;
    key_action.action = action;
    auto iterator = keymaps.find(key_action);
    if (iterator == keymaps.end())
        return;
    keymaps[key_action]();
}
