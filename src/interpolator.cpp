#include "interpolator.hpp"

Interpolator::Interpolator(glm::vec4 start, glm::vec4 end, float duration)
    : start(start), end(end), duration(duration), currentPosition(start),
      progressAmount(0.0f) {}

void Interpolator::progress(float time) {
    set_progress(progressAmount + (time/this->duration));
    updateCurrentPosition();
}

void Interpolator::resetProgress() {
    this->progressAmount = 0;
    this->currentPosition = start;
}

void Interpolator::fillProgress() {
    this->progressAmount = 1.0f;
    this->currentPosition = end;
};

glm::vec4 Interpolator::get_currentPosition() {
    return currentPosition;
}

float Interpolator::get_progress() {
    return progressAmount;
}

bool Interpolator::isFinished() {
    return this->progressAmount >= 1.0f;
}


void Interpolator::set_progress(float amount) {
    this->progressAmount = amount;
    
    // Keep progress bound
    if (this->progressAmount > 1.0f) {
        this->progressAmount = 1.0f;
    } else if (this->progressAmount < 0.0f) {
        this->progressAmount = 0.0f;
    }
}

LinearInterpolator::LinearInterpolator(glm::vec4 start, glm::vec4 end, float duration) : Interpolator(start, end, duration) {}

LinearInterpolator::LinearInterpolator(BezierPath_Linear path, float duration) : Interpolator(path.start, path.end, duration) {}

void LinearInterpolator::updateCurrentPosition() {
    this->currentPosition = start + ((end - start) * progressAmount);
}

void LinearInterpolator::set_path(BezierPath_Linear path) {
    this->start  = path.start;
    this->end = path.end;

    resetProgress();
}

QuadraticInterpolator::QuadraticInterpolator(glm::vec4 start, glm::vec4 control, glm::vec4 end, float duration) : Interpolator(start, end, duration), control(control) {}

QuadraticInterpolator::QuadraticInterpolator(BezierPath_Quadratic path, float duration) : Interpolator(path.start, path.end, duration), control(path.control) {}

// Uses a bezier curve to interpolate between start and end with a control point
void QuadraticInterpolator::updateCurrentPosition() {
    const glm::vec4 position_start_control = start + ((control - start) * progressAmount);
    const glm::vec4 position_control_end = control + ((end - control) * progressAmount);

    this->currentPosition = position_start_control + ((position_control_end - position_start_control) * progressAmount);
}


void QuadraticInterpolator::set_path(BezierPath_Quadratic path) {
    this->start  = path.start;
    this->end = path.end;
    this->control = path.control;

    resetProgress();
}
