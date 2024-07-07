
#include "engine/EngineObject/camera/dollyCamera.hpp"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include "interpolator.hpp"
#include "matrices.hpp"
#include <iostream>

DollyCamera::DollyCamera(BezierPath_Quadratic cameraPath, float cameraDuration,
                         GameObject *target, BezierPath_Quadratic targetPath,
                         float targetDuration, BezierPath_Quadratic lensPath)
    : Camera(cameraPath.start, target),
      positionInterpolator(cameraPath, cameraDuration),
      targetInterpolator(targetPath, targetDuration),
      lensInterpolator(lensPath, cameraDuration),
      isMoving(false) {
    set_progress(0.0f);
}

void DollyCamera::set_progress(float progress) {
    positionInterpolator.set_progress(progress);
    targetInterpolator.set_progress(progress);
    lensInterpolator.set_progress(progress);
}

void DollyCamera::startMoving() { isMoving = true; }

void DollyCamera::stopMoving() { isMoving = false; }

void DollyCamera::update(GLfloat deltaTime) {
    if (isMoving) {
        positionInterpolator.progress(deltaTime);
        targetInterpolator.progress(deltaTime);
        lensInterpolator.progress(deltaTime);

        this->set_position(positionInterpolator.get_currentPosition());
        target->set_position(targetInterpolator.get_currentPosition());
        
        const glm::vec4 lensStatus = lensInterpolator.get_currentPosition();
        this->fov = lensStatus.x;
        this->nearPlane = lensStatus.y;
        this->farPlane = lensStatus.z;


        // Stop updating when not necessary
        isMoving = !(positionInterpolator.isFinished() && targetInterpolator.isFinished());
    }
}