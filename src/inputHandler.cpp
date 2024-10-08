#include "engine/Input/inputHandler.hpp"
#include <iostream>

InputHandler InputHandler::keyMaps = InputHandler();
InputHandler InputHandler::mouseClickMaps = InputHandler();
std::vector<std::function<void(int,int)>> InputHandler::scrollCallbacks = std::vector<std::function<void(int,int)>>();
glm::vec2 InputHandler::mousePos = {0.0f, 0.0f};

void InputHandler::handleKeyInput(GLFWwindow *window, int key, int scan_code,
                                  int action, int mod) {
    auto iterator = keyMaps.find(key);
    if (iterator == keyMaps.end())
        return;
    keyMaps.at(key)(action);
}

void InputHandler::handleMouseClick(GLFWwindow *window, int button, int action,
                                    int mods) {
    auto iterator = mouseClickMaps.find(button);
    if (iterator == mouseClickMaps.end())
        return;
    mouseClickMaps.at(button)(action);
}

void InputHandler::addKeyMapping(Key data, std::function<void(Action)> action) {
    keyMaps.emplace(data, action);
}

void InputHandler::addClickMapping(Key data,
                                   std::function<void(Action)> action) {
    mouseClickMaps.emplace(data, action);
}

void InputHandler::handleCursorPosChange(GLFWwindow *window, double xpos,
                                         double ypos) {
    mousePos = {xpos, ypos};
}

void InputHandler::addScrollCallback(std::function<void(int, int)> callback) {
    scrollCallbacks.push_back(callback);
}

void InputHandler::handleScroll(GLFWwindow *window, double xoffset,
                                double yoffset) {
    for (auto callback : scrollCallbacks) {
        callback(xoffset, yoffset);
    }
}