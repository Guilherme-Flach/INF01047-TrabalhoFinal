#ifndef ENGINEOBJECT_HEADER
#define ENGINEOBJECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "basis.hpp"

class EngineObject {

  protected:
    EngineObject(glm::vec4 position);
    EngineObject(glm::vec4 position, EngineObject *parent);
    glm::vec4 position;
    Basis basis;
    std::vector<EngineObject> children;
    EngineObject *parent;

  public:
    std::vector<EngineObject> get_children();
    EngineObject *get_parent();
    glm::vec4 get_position();
    glm::vec4 get_global_position();

    void set_parent(EngineObject *parent);
    void set_position(glm::vec4 position);

    void addChild(EngineObject *child);
    void translate(glm::vec4 offset);
    void rotate(float angle_x, float angle_y, float angle_z);
};

#endif // GAMEOBJECT_HEADER
