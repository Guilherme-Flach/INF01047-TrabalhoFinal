#ifndef DOLLYCAMERA_HEADER
#define DOLLYCAMERA_HEADER
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include "engine/interpolator.hpp"

class DollyCamera : public Camera {

  protected:
    QuadraticInterpolator cameraInterpolator;
    QuadraticInterpolator targetInterpolator;
    QuadraticInterpolator lensInterpolator;

    bool isMoving;

  public:
    DollyCamera(BezierPath_Quadratic cameraPath, float cameraDuration,
                GameObject *target, BezierPath_Quadratic targetPath,
                float targetDuration, BezierPath_Quadratic lensPath);

    void set_progress(float progress);
    void set_cameraPath(BezierPath_Quadratic path);
    void set_targetPath(BezierPath_Quadratic path);
    void set_lensPath(BezierPath_Quadratic path);


    void startMoving();
    void stopMoving();


    void update(GLfloat deltaTime);
};
#endif // DOLLYCAMERA
