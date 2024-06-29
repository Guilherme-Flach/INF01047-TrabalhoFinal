#ifndef KEYMAP_HEADER
#define KEYMAP_HEADER
#include <functional>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <map>
#include "GLFW/glfw3.h"

typedef int Key;
typedef int Action;

class KeyMap : public std::map<Key, std::function<void(Action)>> {
  private:
        KeyMap() : std::map<Key, std::function<void(Action)>>() {}
        static KeyMap instance;

  public:
    static void addKeyMapping(Key data, std::function<void(Action)> action) {
        instance.emplace(data, action);
    }

    static void handleKeymaps(GLFWwindow *window, int key, int scan_code, int action, int mod) {
        auto iterator = instance.find(key);
        if (iterator == instance.end())
            return;
        instance.at(key)(action);
    }

    static KeyMap &getInstance() {
        static KeyMap *instance = new KeyMap();
        return *instance;
    }

};



#endif // KEYMAP_HEADER