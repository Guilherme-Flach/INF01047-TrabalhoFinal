#include "engine/Input/inputHandler.hpp"

InputHandler InputHandler::keyMaps = InputHandler();
InputHandler InputHandler::mouseClickMaps = InputHandler();
glm::vec2 InputHandler::mousePos = {0.0f,0.0f};
glm::vec2 InputHandler::prevMousePos = {0.0f,0.0f};