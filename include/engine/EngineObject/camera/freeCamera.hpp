#ifndef FREECAMERA_HEADER
#define FREECAMERA_HEADER
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include "engine/interpolator.hpp"

class FreeCamera : public Camera {

  protected:

  public:
    FreeCamera(glm::vec4 position, glm::vec4 lookTowards);
    ~FreeCamera();
};
#endif // FREECAMERA
