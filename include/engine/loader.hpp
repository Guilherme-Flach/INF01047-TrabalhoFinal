#ifndef LOADER_HEADER
#define LOADER_HEADER

#include <functional>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Input/inputHandler.hpp"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include <utils.h>
#include <matrices.hpp>
#include <vector>

class Loader {
  public:
    enum StateFlag {
      VIEW_TYPE
    };

    enum StateValue {
      VIEW_PANORAMIC, VIEW_SHIP
    };

  private:
    static float delta_t;
    static GLFWwindow *window;
    static Camera *active_camera;
    static std::map<StateFlag, StateValue> stateFlags;

    std::vector<GameObject *> game_object_store;
    std::vector<Camera *> camera_store;
  public:
    int program_id = 0;
    InputHandler *inputHandler;

    Loader(int width, int height, char title[]);

    void add_game_object(GameObject &object);
    void add_camera(Camera &camera);

    static float get_delta_t() { return Loader::delta_t; }

    void start(std::function<void(void)> act);

    static Camera *get_active_camera() { return active_camera; }
    static GLFWwindow *get_window() { return window; }
    static void set_active_camera(Camera *camera);

    static void set_globalState(StateFlag flag, StateValue value) { stateFlags[flag] = value; };
    static StateValue get_globalState(StateFlag flag) { return stateFlags[flag]; };

};
#endif // LOADER_HEADER
