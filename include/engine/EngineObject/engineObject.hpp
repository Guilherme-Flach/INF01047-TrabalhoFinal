#ifndef ENGINEOBJECT_HEADER
#define ENGINEOBJECT_HEADER
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>

class EngineObject {

  protected:
    EngineObject(glm::vec4 position);
    EngineObject(glm::vec4 position, EngineObject *parent);
    glm::vec4 position;
    glm::vec4 rotation;
    std::vector<EngineObject> children;
    EngineObject *parent;

  public:
    std::vector<EngineObject> get_children();
    EngineObject *get_parent();
    glm::vec4 get_position();
    glm::vec4 get_globalPosition();

    void set_parent(EngineObject *parent);
    void set_position(glm::vec4 position);

    void addChild(EngineObject *child);
    void translate(glm::vec4 offset);
};

#endif // GAMEOBJECT_HEADER
