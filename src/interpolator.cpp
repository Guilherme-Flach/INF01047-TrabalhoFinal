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

bool Interpolator::get_isFinished() {
    return this->progressAmount >= 1.0f;
}


void Interpolator::set_progress(float amount) {
    this->progressAmount += amount;
    
    // Keep progress bound
    if (this->progressAmount > 1.0f) {
        this->progressAmount = 1.0f;
    } else if (this->progressAmount < 0.0f) {
        this->progressAmount = 0.0f;
    }
}

LinearInterpolator::LinearInterpolator(glm::vec4 start, glm::vec4 end, float duration) : Interpolator(start, end, duration) {}

void LinearInterpolator::updateCurrentPosition() {
    this->currentPosition = start + ((start - end) * progressAmount);
}

QuadradicInterpolator::QuadradicInterpolator(glm::vec4 start, glm::vec4 control, glm::vec4 end, float duration) : Interpolator(start, end, duration), control(control) {}

// Uses a bezier curve to interpolate between start and end with a control point
void QuadradicInterpolator::updateCurrentPosition() {
    const glm::vec4 position_start_control = start + ((control - start) * progressAmount);
    const glm::vec4 position_control_end = control + ((end - control) * progressAmount);

    this->currentPosition = start + (position_control_end - position_start_control) * progressAmount;
}
