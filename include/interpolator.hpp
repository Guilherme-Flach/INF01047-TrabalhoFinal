#ifndef INTERPOLATOR_HEADER
#define INTERPOLATOR_HEADER
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"

class Interpolator {
  protected:
    glm::vec4 start;
    glm::vec4 end;
    glm::vec4 currentPosition;
    float progressAmount;
    float duration;

    Interpolator(glm::vec4 start, glm::vec4 end, float duration);
    virtual void updateCurrentPosition() = 0;

    public:
    void progress(float time);
    void resetProgress();
    void fillProgress();

    glm::vec4 get_currentPosition();
    float get_position();
    float get_progress();
    bool get_isFinished();
    
    void set_progress(float amount);
};

class LinearInterpolator : public Interpolator {
  protected:
    void updateCurrentPosition() override;
  public:
    LinearInterpolator(glm::vec4 start, glm::vec4 end, float duration);
};

class QuadradicInterpolator : public Interpolator {
  protected:
    glm::vec4 control;
    void updateCurrentPosition() override;

  public:
    QuadradicInterpolator(glm::vec4 start, glm::vec4 control, glm::vec4 end, float duration);
};

#endif //INTERPOLATOR