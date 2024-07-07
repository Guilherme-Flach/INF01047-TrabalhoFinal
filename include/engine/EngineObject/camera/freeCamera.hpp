#ifndef FREECAMERA_HEADER
#define FREECAMERA_HEADER
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include "engine/interpolator.hpp"

class FreeCamera : public Camera {

  protected:
    float sensitivity = 1.0f;

  public:
    constexpr const static float heightLimit = 0.97f;
    FreeCamera(glm::vec4 position, glm::vec4 lookTowards);
    ~FreeCamera();

    float get_sensitivity() { return sensitivity; };
    void  set_sensitivity(float sensitivity) { this->sensitivity = sensitivity; };
};
#endif // FREECAMERA
