#ifndef INPUTHANDLER_HEADER
#define INPUTHANDLER_HEADER
#include <functional>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <map>
#include "GLFW/glfw3.h"
#include <vector>

typedef int Key;
typedef int Action;

class InputHandler : public std::map<Key, std::function<void(Action)>> {
  private:
    InputHandler() : std::map<Key, std::function<void(Action)>>() {}
    static InputHandler keyMaps;
    static InputHandler mouseClickMaps;
    static glm::vec2 mousePos;
    static std::vector<std::function<void(int, int)>> scrollCallbacks;

  public:
    static void addKeyMapping(Key data, std::function<void(Action)> action);

    static void addClickMapping(Key data, std::function<void(Action)> action);

    static void addScrollCallback(std::function<void(int,int)> callback);

    static void handleKeyInput(GLFWwindow *window, int key, int scan_code,
                               int action, int mod);

    static void handleScroll(GLFWwindow *window, double xoffset,
                             double yoffset);

    static void handleMouseClick(GLFWwindow *window, int button, int action,
                                 int mods);

    static void handleCursorPosChange(GLFWwindow *window, double xpos,
                                      double ypos);

    static glm::vec2 getMousePos() { return mousePos; }
};

#endif // KEYMAP_HEADER