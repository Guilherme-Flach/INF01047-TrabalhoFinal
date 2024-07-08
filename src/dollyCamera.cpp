
#include "engine/EngineObject/camera/dollyCamera.hpp"
#include "engine/EngineObject/camera/camera.hpp"
#include "engine/EngineObject/gameObject.hpp"
#include "glm/ext/vector_float4.hpp"
#include "engine/interpolator.hpp"


DollyCamera::DollyCamera(BezierPath_Quadratic cameraPath, float cameraDuration,
                         GameObject *target, BezierPath_Quadratic targetPath,
                         float targetDuration, BezierPath_Quadratic lensPath)
    : Camera(cameraPath.start, target),
      cameraInterpolator(cameraPath, cameraDuration),
      targetInterpolator(targetPath, targetDuration),
      lensInterpolator(lensPath, cameraDuration),
      isMoving(false) {
    set_progress(0.0f);
}

void DollyCamera::set_progress(float progress) {
    cameraInterpolator.set_progress(progress);
    targetInterpolator.set_progress(progress);
    lensInterpolator.set_progress(progress);
}

void DollyCamera::startMoving() { isMoving = true; }

void DollyCamera::stopMoving() { isMoving = false; }

void DollyCamera::update(GLfloat deltaTime) {
    if (isMoving) {
        cameraInterpolator.progress(deltaTime);
        targetInterpolator.progress(deltaTime);
        lensInterpolator.progress(deltaTime);

        this->set_position(cameraInterpolator.get_currentPosition());
        target->set_position(targetInterpolator.get_currentPosition());
        
        const glm::vec4 lensStatus = lensInterpolator.get_currentPosition();
        this->fov = lensStatus.x;
        this->nearPlane = lensStatus.y;
        this->farPlane = lensStatus.z;


        // Stop updating when not necessary
        isMoving = !(cameraInterpolator.isFinished() && targetInterpolator.isFinished());
    }

    Camera::update(deltaTime);
}

void DollyCamera::set_cameraPath(BezierPath_Quadratic path) {
    this->cameraInterpolator.set_path(path);
};


void DollyCamera::set_targetPath(BezierPath_Quadratic path) {
    this->targetInterpolator.set_path(path);
};

void DollyCamera::set_lensPath(BezierPath_Quadratic path) {
    this->lensInterpolator.set_path(path);
};

bool DollyCamera::get_isFinished() {
    return this->cameraInterpolator.isFinished() && this->targetInterpolator.isFinished();
}

bool DollyCamera::get_isMoving() {
    return this->isMoving;
}